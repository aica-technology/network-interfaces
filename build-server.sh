#!/bin/bash
ROS_VERSION=foxy

IMAGE_NAME=aica-technology/network-interfaces
IMAGE_STAGE=core-dependencies

SERVE_REMOTE=false
REMOTE_SSH_PORT=4420

BUILD_FLAGS=()

HELP_MESSAGE="Usage: ./build.sh [-t] [-r] [-v] [-s]

Build a Docker container for remote development and/or running unittests.

Options:
  -t, --test               Build and run all the unittests.

  -r, --rebuild            Rebuild the image with no cache.

  -v, --verbose            Show all the output of the Docker
                           build process

  -s, --serve              Start the remove development server.

  -h, --help               Show this help message.
"

while [ "$#" -gt 0 ]; do
  case "$1" in
    -t|--test) IMAGE_STAGE=build-test; shift 1;;
    -r|--rebuild) BUILD_FLAGS+=(--no-cache); shift 1;;
    -v|--verbose) BUILD_FLAGS+=(--progress=plain); shift 1;;
    -s|--serve) SERVE_REMOTE=true ; shift ;;
    -h|--help) echo "${HELP_MESSAGE}"; exit 0;;
    *) echo "Unknown option: $1" >&2; echo "${HELP_MESSAGE}"; exit 1;;
  esac
done

BUILD_FLAGS+=(--build-arg ROS_VERSION="${ROS_VERSION}")
BUILD_FLAGS+=(-t "${IMAGE_NAME}:${IMAGE_STAGE}")
BUILD_FLAGS+=(--target "${IMAGE_STAGE}")

docker pull ghcr.io/aica-technology/ros2-control-libraries:"${ROS_VERSION}"
DOCKER_BUILDKIT=1 docker build "${BUILD_FLAGS[@]}" .

if [ "${SERVE_REMOTE}" = true ]; then
  aica-docker server "${IMAGE_NAME}:${IMAGE_STAGE}" -u ros2 -p "${REMOTE_SSH_PORT}"
fi