cmake_minimum_required(VERSION 3.15)

set(COMMUNICATION_INTERFACES_VERSION 0.0.1)
project(communication_interfaces VERSION ${COMMUNICATION_INTERFACES_VERSION})

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

macro(add_project_dependency)
    find_package(${ARGV})
    if(PKG_EXTERNAL_DEPS)
        set(PKG_EXTERNAL_DEPS "${PKG_EXTERNAL_DEPS}, ${ARGV0} >= ${ARGV1}")
    else()
        set(PKG_EXTERNAL_DEPS "${ARGV0} >= ${ARGV1}")
    endif()
endmacro()

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(FindPkgConfig)

if (NOT cppzmq_POPULATED) # provided by parent CMakeLists.txt (if used)
  add_project_dependency(cppzmq 4.7.1 REQUIRED)
endif()

add_library(${PROJECT_NAME} SHARED
        ${PROJECT_SOURCE_DIR}/src/sockets/UDPSocket.cpp
        ${PROJECT_SOURCE_DIR}/src/sockets/UDPClient.cpp
        ${PROJECT_SOURCE_DIR}/src/sockets/UDPServer.cpp
        ${PROJECT_SOURCE_DIR}/src/sockets/ZMQSocket.cpp
        ${PROJECT_SOURCE_DIR}/src/sockets/ZMQPublisher.cpp
        ${PROJECT_SOURCE_DIR}/src/sockets/ZMQSubscriber.cpp
        ${PROJECT_SOURCE_DIR}/src/sockets/ZMQPublisherSubscriber.cpp
        ${PROJECT_SOURCE_DIR}/src/sockets/TCPSocket.cpp
        ${PROJECT_SOURCE_DIR}/src/sockets/TCPClient.cpp
        ${PROJECT_SOURCE_DIR}/src/sockets/TCPServer.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(${PROJECT_NAME} PUBLIC cppzmq)

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

# export the target and its associated config (includes, linked libraries, etc)
install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Targets.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# generate the version file for the config file
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        VERSION "${PROJECT_VERSION}"
        COMPATIBILITY SameMajorVersion
)

# create config file
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}-config.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
        )

# install config files
install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
        )

if(${PKG_CONFIG_FOUND})
    set(PKG_NAME ${PROJECT_NAME})
    set(PKG_DESC "Communication Interfaces")
    set(pkg_conf_file "${PROJECT_NAME}.pc")
    configure_file("${pkg_conf_file}.in" "${CMAKE_BINARY_DIR}/${pkg_conf_file}" @ONLY)
    install(FILES "${CMAKE_BINARY_DIR}/${pkg_conf_file}"
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig/ COMPONENT pkgconfig)
else()
    message(WARNING "Could not find pkg-config executable, skipping generation of pkg-config files.")
endif()
