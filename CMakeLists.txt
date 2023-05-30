cmake_minimum_required(VERSION 3.15)
project(all_network_interfaces)

include(FetchContent)
option(CPPZMQ_BUILD_TESTS OFF)
FetchContent_Declare(
  cppzmq
  GIT_REPOSITORY https://github.com/zeromq/cppzmq/
  GIT_TAG        v4.7.1
)
FetchContent_MakeAvailable(cppzmq)

add_subdirectory(cpp)
add_subdirectory(source/communication_interfaces)

if(BUILD_TESTING)
  # reexport the test target defined in the subdirectories
  enable_testing()
endif()
