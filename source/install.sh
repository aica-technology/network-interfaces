#!/bin/bash
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" &>/dev/null && pwd)"

INSTALL_DESTINATION="/usr/local"
AUTO_INSTALL=""
CPPZMQ_VERSION=4.7.1

HELP_MESSAGE="Usage: [sudo] ./install.sh [OPTIONS]
An install script for the communication_interfaces library.
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

  cd "${SCRIPT_DIR}" && rm -rf "${SCRIPT_DIR}"/install || exit 1
}

install_communication_interfaces () {
  cd "${SCRIPT_DIR}"/communication_interfaces && mkdir -p build && cd build || exit 1
  cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="${INSTALL_DESTINATION}" .. || exit 1

  make -j && make install || exit 1
  ldconfig

  cd "${SCRIPT_DIR}" || exit 1
}

function uninstall() {
  function delete_components() {
    rm -r "${INSTALL_DESTINATION}"/include/communication_interfaces
    rm -r "${INSTALL_DESTINATION}"/lib/libcommunication_interfaces*.so
  }

  delete_components >/dev/null 2>&1

  echo "Deleted any communication_interfaces artefacts from ${INSTALL_DESTINATION}."
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

echo ">>> INSTALLING ZMQ DEPENDENCIES"
install_cppzmq || exit 1

echo ">>> INSTALLING COMMUNICATION INTERFACES"
install_communication_interfaces || exit 1

echo ">>> DONE"
