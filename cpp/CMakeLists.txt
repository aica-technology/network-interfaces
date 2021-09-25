cmake_minimum_required(VERSION 3.9)
project(network_interfaces VERSION 0.0)

option(BUILD_TESTING "Build tests." OFF)

# Default to C99
if(NOT CMAKE_C_STANDARD)
    set(CMAKE_C_STANDARD 99)
endif()

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

if(BUILD_TESTING)
    enable_testing()
    find_package(GTest REQUIRED)
else()
    find_package(GTest QUIET)
endif()

find_library(clproto REQUIRED)
find_library(state_representation REQUIRED)

find_package(cppzmq REQUIRED)

include_directories(
        include
)

install(DIRECTORY include/
        DESTINATION include
)

if(BUILD_TESTING)
    add_executable(test_communication test/test_communication.cpp)
    target_link_libraries(test_communication ${GTEST_LIBRARIES} pthread)
    add_test(NAME test_communication COMMAND test_communication)
    target_link_libraries(test_communication clproto cppzmq state_representation)
endif()
