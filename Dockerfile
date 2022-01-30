FROM ghcr.io/epfl-lasa/control-libraries/development-dependencies as source-dependencies

RUN apt-get update && apt-get install -y libmbedtls-dev libsodium-dev libzmq3-dev \
  && apt-get clean && rm -rf /var/lib/apt/lists/*

WORKDIR /tmp
ARG CPPZMQ_VERSION=4.7.1
RUN wget https://github.com/zeromq/cppzmq/archive/v${CPPZMQ_VERSION}.tar.gz -O cppzmq-${CPPZMQ_VERSION}.tar.gz
RUN tar -xzf cppzmq-${CPPZMQ_VERSION}.tar.gz
WORKDIR /tmp/cppzmq-${CPPZMQ_VERSION}
RUN mkdir build && cd build && cmake .. -DCPPZMQ_BUILD_TESTS=OFF && make -j install

WORKDIR /tmp
ARG CONTROL_LIBRARIES_BRANCH=develop
RUN git clone -b ${CONTROL_LIBRARIES_BRANCH} --single-branch https://github.com/epfl-lasa/control-libraries.git
RUN cd control-libraries/source && sudo ./install.sh --auto --no-controllers --no-dynamical-systems --no-robot-model
RUN cd control-libraries/protocol && sudo ./install.sh --auto
RUN pip3 install control-libraries/python

RUN rm -rf /tmp/*

# install pyzmq
RUN pip3 install pyzmq

FROM source-dependencies as build-test

WORKDIR ${HOME}
COPY --chown=${USER} ./cpp ./network_interfaces/cpp
COPY --chown=${USER} ./python ./network_interfaces/python

RUN cd ./network_interfaces/cpp && mkdir build && cd build && cmake -DBUILD_TESTING=ON .. \
  && make -j && CTEST_OUTPUT_ON_FAILURE=1 make test && make -j install
RUN cd ./network_interfaces/python && pip3 install ./ && python3 -m unittest discover ./test --verbose

# Clean image
RUN rm -rf ./network_interfaces
