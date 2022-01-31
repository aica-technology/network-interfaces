# CHANGELOG

Release Versions:

- [0.2.0](#020)
- [0.1.0](#010)

## 0.2.0

Version 0.2.0 introduces a slightly different Dockerfile and contains a GitHub workflow to build and push an image with
the pre-built network interface libraries. Additionally, two loopback scripts have been added to simplify communication
testing.

## 0.1.0

A set of drivers, protocols and libraries for communicating between software and hardware devices.

### Features

**ZMQ Interface**

A network interface with ZMQ subscribers and publishers, implemented as a library in both C++ and Python. The library
defines the message structures using `state_representation` types and the encoding / decoding of the messages
using `clproto`.
