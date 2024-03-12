#!/bin/bash

IMAGE_NAME=ghcr.io/aica-technology/network-interfaces
IMAGE_TAG=latest

ROS2_VERSION=iron

HELP_MESSAGE="Usage: build.sh [options]
Options:
  --test                    Target the test layer to run the tests.

  --ros2-version <VERSION>  Specify the version of ROS 2 to use.
                            (default: $ROS2_VERSION)

  -v|--verbose              Set the build output to verbose.

  --cache-id <id>           Invalidate the mount cache (e.g. CMake build folder)
                            by providing a new value.

  -r|--no-cache             Invalidate all cache (layer + mount).

  -h|--help                 Show this help message.
"

TEST=0
BUILD_FLAGS=()
while [ "$#" -gt 0 ]; do
  case "$1" in
    --test) BUILD_FLAGS+=(--target=test); TEST=1; IMAGE_TAG=test; shift 1;;
    --ros2-version) ROS2_VERSION=$2; shift 2;;
    -v|--verbose) BUILD_FLAGS+=(--progress=plain); shift 1;;
    --cache-id) BUILD_FLAGS+=(--build-arg CACHEID=$2); shift 2;;
    -r|--no-cache) BUILD_FLAGS+=(--no-cache); BUILD_FLAGS+=(--build-arg CACHEID=$(date +%s)); shift 1;;
    -h|--help) echo "$HELP_MESSAGE"; exit 0;;
    -*) echo "Unknown option: $1" >&2; echo "$HELP_MESSAGE"; exit 1;;
  esac
done

BUILD_FLAGS+=(--build-arg ROS2_VERSION="${ROS2_VERSION}")
BUILD_FLAGS+=(-t "${IMAGE_NAME}:${IMAGE_TAG}")

DOCKER_BUILDKIT=1 docker build "${BUILD_FLAGS[@]}" . || exit 1
