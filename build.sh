#!/bin/bash
ROS_VERSION=foxy

IMAGE_NAME=aica-technology/network-interfaces
IMAGE_STAGE=core-dependencies

BUILD_FLAGS=()

HELP_MESSAGE="Usage: ./build.sh [--rebuild] [--test] [--verbose]

Build a Docker container for remote development and/or running unittests.

Options:
  -r, --rebuild            Rebuild the image with no cache.

  -t, --test               Build and run all the unittests.

  -v, --verbose            Show all the output of the Docker
                           build process

  -h, --help               Show this help message."

while [ "$#" -gt 0 ]; do
  case "$1" in
    -r|--rebuild) BUILD_FLAGS+=(--no-cache); shift 1;;
    -t|--test) IMAGE_STAGE=build-test; shift 1;;
    -v|--verbose) BUILD_FLAGS+=(--progress=plain); shift 1;;
    -h|--help) echo "${HELP_MESSAGE}"; exit 0;;
    *) echo "Unknown option: $1" >&2; echo "${HELP_MESSAGE}"; exit 1;;
  esac
done

BUILD_FLAGS+=(--build-arg ROS_VERSION="${ROS_VERSION}")
BUILD_FLAGS+=(-t "${IMAGE_NAME}:${IMAGE_STAGE}")
BUILD_FLAGS+=(--target "${IMAGE_STAGE}")

docker pull ghcr.io/aica-technology/ros2-control-libraries:"${ROS_VERSION}"
DOCKER_BUILDKIT=1 docker build "${BUILD_FLAGS[@]}" .