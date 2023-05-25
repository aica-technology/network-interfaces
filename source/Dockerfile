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

RUN rm -rf /tmp/*
