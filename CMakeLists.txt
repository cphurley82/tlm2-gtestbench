cmake_minimum_required(VERSION 3.10)
project(tlm2-gtestbench VERSION 0.1
        DESCRIPTION "TLM2 testbench using Google's C++ test framework."
        LANGUAGES CXX)

add_executable(run-tests src/run_tests.cc
        src/systemc_fixture.cc src/systemc_fixture.h
        src/systemc_testbench.cc src/systemc_testbench.h
        src/memory_fixture.cc src/memory_fixture.h
        src/memory.cc src/memory.h
        src/memory_testbench.cc src/memory_testbench.h
        src/memory_test.cc)
target_include_directories(run-tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# SystemC
add_subdirectory(ext/systemc)
target_include_directories(run-tests PRIVATE ext/systemc/src)
target_link_libraries(run-tests systemc)

# GTest
add_subdirectory(ext/googletest)
target_link_libraries(run-tests gtest_main)

# Use C++14
set_property(TARGET run-tests systemc gtest
        PROPERTY CXX_STANDARD 14)

# CTest
add_test(NAME run-tests COMMAND run-tests)
