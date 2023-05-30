cmake_minimum_required(VERSION 3.15)

set (NETWORK_INTERFACES_VERSION 1.2.0)
project(network_interfaces VERSION ${NETWORK_INTERFACES_VERSION})

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

add_project_dependency(control_libraries 7.0.0 REQUIRED COMPONENTS state_representation)
add_project_dependency(clproto 7.0.0 REQUIRED)
if (NOT cppzmq_POPULATED) # provided by parent CMakeLists.txt (if used)
  add_project_dependency(cppzmq 4.7.1 REQUIRED)
endif()

include_directories(include)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE include)
target_link_libraries(${PROJECT_NAME} INTERFACE clproto cppzmq state_representation)

install(DIRECTORY include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

add_executable(zmq_loopback_state scripts/zmq_loopback_state.cpp)
target_link_libraries(zmq_loopback_state clproto cppzmq state_representation)

add_executable(zmq_loopback_command scripts/zmq_loopback_command.cpp)
target_link_libraries(zmq_loopback_command clproto cppzmq state_representation)

if(BUILD_TESTING)
    add_executable(test_zmq_communication test/test_zmq_communication.cpp)
    target_link_libraries(test_zmq_communication ${GTEST_LIBRARIES} pthread clproto cppzmq state_representation)
    add_test(NAME test_zmq_communication COMMAND test_zmq_communication)
endif()

# generate the version file for the config file
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  VERSION "${PROJECT_VERSION}"
  COMPATIBILITY SameMajorVersion
)

# create config file
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/network_interfaces-config.cmake.in
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
  set(PKG_DESC "Network Interfaces")
  set(pkg_conf_file "network_interfaces.pc")
  configure_file("${pkg_conf_file}.in" "${CMAKE_BINARY_DIR}/${pkg_conf_file}" @ONLY)
  install(FILES "${CMAKE_BINARY_DIR}/${pkg_conf_file}"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig/ COMPONENT pkgconfig)
else()
  message(WARNING "Could not find pkg-config executable, skipping generation of pkg-config files.")
endif()
