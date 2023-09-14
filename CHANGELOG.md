# CHANGELOG

Release Versions:

- [1.4.1](#141)
- [1.4.0](#140)
- [1.3.0](#130)
- [1.2.0](#120)
- [1.1.0](#110)
- [1.0.0](#100)
- [0.2.0](#020)
- [0.1.0](#010)

## Upcoming changes

- refactor!: remove network interfaces (#56)
- feat: add python bindings and tests (#57)

## 1.4.1

Version 1.4.1 includes two fixes to the CI that are required downstream.

## 1.4.0

Version 1.4.0 includes a first stable version of communication interfaces.

## 1.3.0

Version 1.3.0 introduces the first version of the communication_interfaces library, which is a more standalone and
versatile alternative of network_interfaces for socket communication. It doesn't have dependencies on control libraries
or clproto. Additionally, the release includes automated builds for the CI.

## 1.2.0

Version 1.2.0 is an update to support the latest release of
[aica-technology/control-libraries version 7.0](https://github.com/aica-technology/control-libraries/releases/tag/v7.0.0).
The only new feature is a pair of configuration files that lets `pkg-config` find the C++ library if it's installed.

## 1.1.0

Version 1.1.0 sets control libraries version 6.0.0 as default and fixes the GitHub workflow. Additionally, the C++
library now has its proper CMake configuration and can be included and linked with `find_package(network_interfaces)`
and`target_link_libraries(${MY_TARGET} ${network_interfaces_LIBRARIES}`.

## 1.0.0

Version 1.0.0 accounts for breaking changes in control libraries and defines 5.0.0 as the required control libraries
version. Additionally, there is now an install script that takes care of the installation of network interfaces.

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
