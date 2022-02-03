#!/bin/bash
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" &>/dev/null && pwd)"

INSTALL_DESTINATION="/usr/local"
BUILD_TESTING="OFF"
AUTO_INSTALL=""
CPPZMQ_VERSION=4.7.1

HELP_MESSAGE="Usage: [sudo] ./install.sh [OPTIONS]
An install script for the network_interfaces library.
Options:
  -y, --auto               Suppress any input prompts and
                           automatically approve install steps.
  -d, --dir [path]         Configure the installation directory
                           (default: ${INSTALL_DESTINATION}).
  --build-tests            Build the unittest targets.
  --clean                  Delete any previously installed header
                           files from ${INSTALL_DESTINATION}/include and any
                           shared library files from ${INSTALL_DESTINATION}/lib.
  --cleandir [path]        Delete any previously installed header
                           and library files from the specified path.
  -h, --help               Show this help message."

function install_cppzmq() {
  apt-get update && apt-get install "${AUTO_INSTALL}" libzmq3-dev || exit 1

  mkdir -p "${SCRIPT_DIR}"/install
  cd "${SCRIPT_DIR}"/install || exit 1
  wget https://github.com/zeromq/cppzmq/archive/v${CPPZMQ_VERSION}.tar.gz -O cppzmq-${CPPZMQ_VERSION}.tar.gz &&
    tar -xzf cppzmq-${CPPZMQ_VERSION}.tar.gz &&
    rm cppzmq-${CPPZMQ_VERSION}.tar.gz

  cd "${SCRIPT_DIR}"/install/cppzmq-"${CPPZMQ_VERSION}" || exit 1
  mkdir build && cd build && cmake .. -DCPPZMQ_BUILD_TESTS=OFF && make -j install || exit 1
  ldconfig

  pip3 install pyzmq

  cd "${SCRIPT_DIR}" && rm -rf "${SCRIPT_DIR}"/install || exit 1
}

install_network_interfaces () {
  cd "${SCRIPT_DIR}"/cpp && mkdir -p build && cd build || exit 1
  cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING="${BUILD_TESTING}" \
    -DCMAKE_INSTALL_PREFIX="${INSTALL_DESTINATION}" .. || exit 1

  make -j && make install || exit 1
  ldconfig

  cd "${SCRIPT_DIR}"/python && pip3 install ./ || exit 1
  if [ ${BUILD_TESTING} == "ON" ]; then
    python3 -m unittest discover ./test --verbose || exit 1
  fi

  cd "${SCRIPT_DIR}" || exit 1
}

function uninstall() {
  function delete_components() {
    rm -r "${INSTALL_DESTINATION}"/include/clproto
    rm -r "${INSTALL_DESTINATION}"/lib/libclproto*.so
    clean_bindings
  }

  delete_components >/dev/null 2>&1

  echo "Deleted any network_interfaces artefacts from ${INSTALL_DESTINATION}."
}

while [ "$#" -gt 0 ]; do
  case "$1" in
  -y|--auto) AUTO_INSTALL="-y"; shift 1;;
  -d|--dir) INSTALL_DESTINATION=$2; shift 2;;
  --build-tests) BUILD_TESTING="ON"; shift 1;;
  --clean) uninstall; exit 0;;
  --cleandir) INSTALL_DESTINATION=$2; uninstall; exit 0;;
  -h|--help) echo "${HELP_MESSAGE}"; exit 0;;
  -*) echo "Unknown option: $1" >&2
      echo "${FAIL_MESSAGE}"
      exit 1;;
  esac
done

STATE_REPRESENTATION_INSTALL=$(ldconfig -p | grep libstate_representation)
if [ -z "${STATE_REPRESENTATION_INSTALL}" ]; then
  echo ">>> STATE REPRESENTATION LIBRARY NOT FOUND!"
  echo ">>> Install state_representation from https://github.com/epfl-lasa/control-libraries/source"
  exit 1
fi

CL_PROTO_INSTALL=$(ldconfig -p | grep libclproto)
if [ -z "${CL_PROTO_INSTALL}" ]; then
  echo ">>> CL PROTO LIBRARY NOT FOUND!"
  echo ">>> Install clproto from https://github.com/epfl-lasa/control-libraries/protocol"
  exit 1
fi

CL_PYTHON_BINDINGS=$(pip3 list | grep control-libraries)
if [ -z "${CL_PYTHON_BINDINGS}" ]; then
  echo ">>> PYTHON BINDINGS FOR CONTROL LIBRARIES NOT FOUND!"
  echo ">>> Install bindings from https://github.com/epfl-lasa/control-libraries/python"
  exit 1
fi

echo ">>> INSTALLING ZMQ DEPENDENCIES"
install_cppzmq || exit 1

echo ">>> INSTALLING NETWORK INTERFACES"
install_network_interfaces || exit 1

echo ">>> DONE"
