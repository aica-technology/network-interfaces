ARG ROS2_VERSION=v1.0.1-iron
FROM ghcr.io/aica-technology/ros2-ws:${ROS2_VERSION} as base
USER ${USER}

FROM base as apt-dependencies
RUN <<HEREDOC
PACKAGES="libzmq3-dev"
mkdir -p /tmp/apt

sudo apt-get update
# We then do a dry-run and parse the output of apt to gather the list of packages to be installed
# Example output:
# ```
# #########
# NOTE: This is only a simulation!
#       apt-get needs root privileges for real execution.
#       Keep also in mind that locking is deactivated,
#       so don't depend on the relevance to the real current situation!
# Reading package lists...
# Building dependency tree...
# Reading state information...
# The following additional packages will be installed:
#   libavutil56 libblosc1
# The following NEW packages will be installed:
#   libavutil56 libblosc1
# 0 upgraded, 5 newly installed, 0 to remove and 28 not upgraded.
# Inst libavutil56 (7:4.4.2-0ubuntu0.22.04.1 Ubuntu:22.04/jammy-updates, Ubuntu:22.04/jammy-security [arm64])
# Inst libblosc1 (1.21.1+ds2-2 Ubuntu:22.04/jammy [arm64])
# Conf libavutil56 (7:4.4.2-0ubuntu0.22.04.1 Ubuntu:22.04/jammy-updates, Ubuntu:22.04/jammy-security [arm64])
# Conf libblosc1 (1.21.1+ds2-2 Ubuntu:22.04/jammy [arm64])
# ```
# Transformed into:
# ```
# libavutil56
# libblosc1
# ```
apt-get install ${PACKAGES} --dry-run \
  | grep -e '^Inst ' \
  | sed -E 's/^Inst (\S+) .*$/\1/' > /tmp/new-packages.txt
# Then we install apt packages like normal
sudo apt-get install -y ${PACKAGES}
# Finally we use dpkg to get all files installed by those packages and copy them to a new root
#  - get list of files installed by all the packages
#  - remove empty lines
#  - sort
#  - remove duplicates
#  - copy files while keeping file hierarchy and preserving links as-is
#  - remove "omitting directory" messages (we don't do recursive copy as we only want specific files) for cleaner output
xargs -a /tmp/new-packages.txt dpkg-query -L \
  | sed '/^$/d' | sort | uniq \
  | xargs -d "\n" cp --parents -dp -t /tmp/apt  2>&1 \
  | grep -v 'omitting directory'
# this root can then be copied to / to install everything globally or use LD_LIBRARY_PATH to use it locally
HEREDOC

FROM base as code
WORKDIR /src
COPY --from=apt-dependencies /tmp/apt /
COPY --chown=${USER}:${USER} ./source/communication_interfaces /src

FROM code as development

FROM code as build
ARG TARGETPLATFORM
ARG CACHEID
RUN --mount=type=cache,target=./build,id=cmake-${TARGETPLATFORM}-${CACHEID},uid=1000 \
  cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build

FROM build as install
ARG TARGETPLATFORM
ARG CACHEID
RUN --mount=type=cache,target=./build,id=cmake-${TARGETPLATFORM}-${CACHEID},uid=1000 \
  cmake --install build --prefix /tmp/communication-interfaces

FROM base as python
ARG TARGETPLATFORM
ARG CACHEID
COPY --from=apt-dependencies /tmp/apt /
COPY --from=install /tmp/communication-interfaces /usr
COPY --chown=${USER}:${USER} ./python /python
RUN --mount=type=cache,target=${HOME}/.cache,id=pip-${TARGETPLATFORM}-${CACHEID},uid=1000 \
  python3 -m pip install --prefix=/tmp/python /python
RUN mkdir -p /tmp/python-home/${USER}/.local/lib/python3.10/ \
  && mv /tmp/python/local/lib/python3.10/dist-packages/ /tmp/python-home/${USER}/.local/lib/python3.10/site-packages/

FROM build as test
ARG TARGETPLATFORM
ARG CACHEID
RUN --mount=type=cache,target=./build,id=cmake-${TARGETPLATFORM}-${CACHEID},uid=1000 \
  cmake -B build -DBUILD_TESTING=ON && cd build && make && CTEST_OUTPUT_ON_FAILURE=1 make test
COPY --from=install /tmp/communication-interfaces /usr/local
COPY --from=python /tmp/python-home/ /home
COPY --from=python /python/test /python/test
RUN python3 -m pytest /python/test --verbose

FROM base as python-stubs
ARG TARGETPLATFORM
ARG CACHEID
COPY --from=apt-dependencies /tmp/apt /
COPY --from=install /tmp/communication-interfaces /usr
COPY --from=python /tmp/python-home /home
RUN sudo pip install pybind11-stubgen
RUN --mount=type=cache,target=${HOME}/.cache,id=pip-${TARGETPLATFORM}-${CACHEID},uid=1000 \
<<HEREDOC
python3 -c "import communication_interfaces"
if [ $? -eq 0 ]; then
  pybind11-stubgen communication_interfaces -o ./stubs
  mv ./stubs/communication_interfaces ./stubs/communication_interfaces-stubs
  cat << EoF > ./stubs/setup.py
from distutils.core import setup

import communication_interfaces


setup(
    name="communication_interfaces-stubs",
    author="Dominic Reber",
    author_email="dominic@aica.tech",
    version=communication_interfaces.__version__,
    package_data={"communication_interfaces-stubs": ["*.pyi"]},
    packages=["communication_interfaces-stubs"]
)
EoF
  python3 -m pip install --prefix=/tmp/python ./stubs || exit 1
  rm -r ./stubs
fi
HEREDOC
RUN mkdir -p /tmp/python-home/${USER}/.local/lib/python3.10/ \
  && mv /tmp/python/local/lib/python3.10/dist-packages/ /tmp/python-home/${USER}/.local/lib/python3.10/site-packages/

FROM scratch as production
COPY --from=apt-dependencies /tmp/apt /
COPY --from=install /tmp/communication-interfaces /usr/local
COPY --from=python /tmp/python-home/ /home
COPY --from=python-stubs /tmp/python-home /home

ARG VERSION
ARG ROS2_VERSION
LABEL org.opencontainers.image.title="AICA network-interfaces library"
LABEL org.opencontainers.image.description="AICA network-interfaces library (for ${ROS2_VERSION})"
LABEL org.opencontainers.image.version="${VERSION}"
LABEL tech.aica.image.metadata='{"type":"lib","base":{"name":"@aica/foss/ros2-ws","version":"'${ROS2_VERSION}'"}}'