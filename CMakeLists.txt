cmake_minimum_required(VERSION 3.10)
project(tlm2-gtestbench VERSION 0.1
                        DESCRIPTION "TLM2 testbench using Google's C++ test framework."
                        LANGUAGES CXX)
add_executable(run-tests run_tests.cc)
