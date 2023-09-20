# CHANGELOG

Release Versions:

- [2.0.1](#201)
- [2.0.0](#200)
- [1.4.1](#141)
- [1.4.0](#140)
- [1.3.0](#130)
- [1.2.0](#120)
- [1.1.0](#110)
- [1.0.0](#100)
- [0.2.0](#020)
- [0.1.0](#010)

## 2.0.1

Version 2.0.1 contains a hotfix that enables socket communiction with any serialized message in Python, which was not
possible before due to an error in the bindings.

- fix(python): avoid removal of null characters in bindings (#66)

## 2.0.0

Version 2.0.0 is a major update to this repository and contains breaking changes.

### Breaking changes

The `network_interfaces` library has been completely removed in favor of `communication_interfaces`.

- refactor!: remove network interfaces (#56)

### Features

The `communication_interfaces` library is now at version 1.0.0 and now comes with Python bindings generated with Pybind.
Additionally, stubs are automatically created for development purposes.

- feat: add python bindings and tests (#57)
- feat(python): add stage to generate python stubs (#59)

### Behind the scenes

- build: rename Dockerfile and update helper scripts (#58)
- ci: add workflow to run tests on PR event (#60, #61)

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
