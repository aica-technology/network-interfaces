cmake_minimum_required(VERSION 3.15)
project(all_network_interfaces)



add_subdirectory(cpp)
add_subdirectory(source/communication_interfaces)

if(BUILD_TESTING)
  # reexport the test target defined in the subdirectories
  enable_testing()
endif()
