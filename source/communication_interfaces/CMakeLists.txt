cmake_minimum_required(VERSION 3.15)

project(communication_interfaces VERSION 0.0.1)

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

include(GNUInstallDirs)

find_package(control_libraries 7.0.0 REQUIRED COMPONENTS state_representation)
find_package(cppzmq 4.7.1 REQUIRED)

include_directories(include)

add_library(${PROJECT_NAME} SHARED
        ${PROJECT_SOURCE_DIR}/src/ByteArray.cpp
        ${PROJECT_SOURCE_DIR}/src/sockets/UDPSocket.cpp
        ${PROJECT_SOURCE_DIR}/src/sockets/UDPClient.cpp
        ${PROJECT_SOURCE_DIR}/src/sockets/UDPServer.cpp
        ${PROJECT_SOURCE_DIR}/src/sockets/ZMQSocket.cpp
        ${PROJECT_SOURCE_DIR}/src/sockets/ZMQPublisher.cpp
        ${PROJECT_SOURCE_DIR}/src/sockets/ZMQSubscriber.cpp
)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PUBLIC cppzmq state_representation)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS ${PROJECT_NAME}
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        )

if(BUILD_TESTING)
    add_executable(test_${PROJECT_NAME} test/test_communication_interfaces.cpp)
    file(GLOB_RECURSE MODULE_TEST_SOURCES test/tests test_*.cpp)
    target_sources(test_${PROJECT_NAME} PRIVATE ${MODULE_TEST_SOURCES})
    target_link_libraries(test_${PROJECT_NAME} ${PROJECT_NAME} ${GTEST_LIBRARIES} pthread)
    add_test(NAME test_${PROJECT_NAME} COMMAND test_${PROJECT_NAME})
endif()
