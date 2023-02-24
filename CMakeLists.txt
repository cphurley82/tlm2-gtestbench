cmake_minimum_required(VERSION 3.10)
project(
  tlm2-gtestbench 
  VERSION 0.1
  DESCRIPTION "TLM2 testbench using Google's C++ test framework."
  LANGUAGES CXX
)

add_executable(
  run-tests
  src/run_tests.cc
  src/systemc_fixture.cc src/systemc_fixture.h
  src/systemc_testbench.cc src/systemc_testbench.h
  src/memory_fixture.cc src/memory_fixture.h
  src/memory.cc src/memory.h
  src/memory_testbench.cc src/memory_testbench.h
  src/memory_test.cc
)

target_include_directories(run-tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

include(FetchContent)

# SystemC
FetchContent_Declare(
  systemc
  URL https://github.com/accellera-official/systemc/archive/refs/tags/2.3.4.zip
)
FetchContent_MakeAvailable(systemc)
target_link_libraries(run-tests systemc)

# GTest
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
target_link_libraries(run-tests gtest_main)

# Use C++20 for all targets, C++11 through C++20 should work but leaving it
# unspecified could cause problems for SystemC.
set_property(TARGET run-tests systemc gtest PROPERTY CXX_STANDARD 20)

# CTest
enable_testing()
add_test(NAME run-tests COMMAND run-tests)
