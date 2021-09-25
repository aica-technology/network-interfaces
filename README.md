# network-interfaces

A repository of drivers, protocols and libraries for communicating between software and hardware devices.

## Usage

Use the C++ projects in one of the two following ways:

- Include the `include` directory of the network interface library in another `CMakeLists.txt` with
  ```cmake
  include_directories(path/to/network-interfaces/cpp/include)
  ```
- Install the network interface library to `/usr/local/include` with
  ```console
  cd path/to/network-interfaces/cpp && mkdir build && cd build && cmake .. && make install
  ```
  and use it in another `CMakeLists.txt` with
  ```cmake
  find_library(network_interfaces REQUIRED)
  ```

For Python, install the library simply with

```console
cd path/to/network-interfaces/python && pip3 install ./
```

## Development

To build and run a Docker container as an SSH toolchain server for remote development with

```console
bash build.sh
aica-docker server aica-technology/network-interfaces -u ros2 -p 8010
```

Note: This requires the installation of the `aica-docker` scripts
from [here](https://github.com/aica-technology/docker-images).

Additionally, to run the tests without an SSH toolchain server, build the image with the `--test` option:

```console
bash build.sh --test
```
