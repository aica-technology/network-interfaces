link_directories(/usr/local/lib)
add_executable(run_tests run_tests.cpp)
target_link_libraries(run_tests ${GTEST_LIBRARIES} pthread)
add_test(NAME run_tests COMMAND run_tests)

file(GLOB_RECURSE MODULE_TEST_SOURCES tests/test_*.cpp)
target_sources(run_tests PRIVATE ${MODULE_TEST_SOURCES})
target_link_libraries(run_tests clproto cppzmq state_representation)