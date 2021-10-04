ARG ROS_VERSION=foxy
FROM ghcr.io/aica-technology/ros2-control-libraries:${ROS_VERSION} as core-dependencies

RUN sudo apt-get update && sudo apt-get install -y \
  libmbedtls-dev \
  libsodium-dev \
  libzmq3-dev \
  && sudo rm -rf /var/lib/apt/lists/*

# install cppzmq bindings
ARG CPPZMQ_VERSION=4.7.1
WORKDIR /tmp
RUN wget https://github.com/zeromq/cppzmq/archive/v${CPPZMQ_VERSION}.tar.gz -O cppzmq-${CPPZMQ_VERSION}.tar.gz
RUN tar -xzf cppzmq-${CPPZMQ_VERSION}.tar.gz
WORKDIR /tmp/cppzmq-${CPPZMQ_VERSION}
RUN mkdir build && cd build && cmake .. -DCPPZMQ_BUILD_TESTS=OFF && make -j install
WORKDIR /tmp
RUN rm -rf cppzmq*

# install pyzmq
RUN pip3 install pyzmq

WORKDIR ${HOME}

# Clean image
RUN sudo apt-get clean && sudo rm -rf /var/lib/apt/lists/*

FROM core-dependencies as build-test

COPY ./ ./
RUN cd cpp && mkdir build && cd build && cmake -DBUILD_TESTING=ON .. && make && CTEST_OUTPUT_ON_FAILURE=1 make test
RUN cd python && pip install ./ && python3 -m unittest discover ./test --verbose
