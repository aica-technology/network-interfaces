FROM ghcr.io/aica-technology/control-libraries/development-dependencies as source-dependencies

RUN apt-get update && apt-get install -y \
  libzmq3-dev \
  && apt-get clean && rm -rf /var/lib/apt/lists/*

WORKDIR /tmp

ARG CPPZMQ_VERSION=4.7.1
RUN wget https://github.com/zeromq/cppzmq/archive/v${CPPZMQ_VERSION}.tar.gz -O cppzmq-${CPPZMQ_VERSION}.tar.gz
RUN tar -xzf cppzmq-${CPPZMQ_VERSION}.tar.gz
WORKDIR /tmp/cppzmq-${CPPZMQ_VERSION}
RUN mkdir build && cd build && cmake .. -DCPPZMQ_BUILD_TESTS=OFF && make -j install


WORKDIR /tmp
ARG CONTROL_LIBRARIES_BRANCH=v7.0.0
RUN git clone -b ${CONTROL_LIBRARIES_BRANCH} --depth 1 https://github.com/aica-technology/control-libraries.git
RUN cd control-libraries/source && ./install.sh --auto --no-controllers --no-dynamical-systems --no-robot-model

RUN rm -rf /tmp/*
