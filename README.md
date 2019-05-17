# TLM2-GTestbench #
SystemC TLM2 Testbench using Google's C++ test framework.

Combines 
[OSCI SystemC Simulator](https://www.accellera.org/downloads/standards/systemc)
and [Google Test](https://github.com/google/googletest) to enable easy testing
and development of TLM2 models.

Example test:

```c++
TEST_F(MemoryFixture, InitalReadIsAllFs) {
  // Create a payload
  auto payload = std::make_shared<tlm::tlm_generic_payload>();
  auto delay = SC_ZERO_TIME;

  // Set payload properties to indicate a read command
  payload->set_command(tlm::TLM_READ_COMMAND);

  // Send payload for processing by the device under test.
  GetTestbench().mirror.socket->b_transport(*payload, delay);

  // Assert the data returned is all 0xf's as expected.
  EXPECT_EQ(*reinterpret_cast<uint64_t*>(payload->get_data_ptr()),
            0xffffffffffffffff);
}
```

## Getting Started

### Setup

Before running tlm2-gtestbench you will need to acquire the dependencies, 
configure and build.

Suppose you put tlm2-gtestbench in directory `${TLM2GTEST_DIR}`. 

### External Dependencies

Download 
[googletest](https://github.com/google/googletest/archive/release-1.8.1.zip) and 
[systemc](https://www.accellera.org/images/downloads/standards/systemc/systemc-2.3.3.zip) 
to the `${TLM2GTEST_DIR}\ext` folder and extract them and rename them to remove
the version. The `${TLM2GTEST_DIR}` directory structure should look like this:

    .
    ├───ext
    │   ├───googletest
    │   │   ├───ci
    │   │   ├───googlemock
    │   │   └───googletest
    │   └───systemc
    │       ├───cmake
    │       └───src
    └───src
    
You can also run `setup.py` to do this for you: 

    cd ${TLM2GTEST_DIR}
    python3 setup.py
    
### Build

#### Using CMake

tlm2-gtestbench comes with a [CMake](http://www.cmake.org) build script 
([CMakeLists.txt](https://github.com/cphurley82/tlm2-gtestbench/blob/master/CMakeLists.txt))
that can be used to configure and build like this from a unix-like shell:

    mkdir build             # Create a directory to hold the build output.
    cd build
    cmake ${TLM2GTEST_DIR}  # Generate build scripts.
    make                    # Build tlm2-gtestbench
    
#### Run

Run simply run the tlm2-gtestbench suite by running `run-tests` from the `build`
directory: 

    $ ./run-tests
    SystemC 2.3.3-Accellera --- May 16 2019 15:30:26
    Copyright (c) 1996-2018 by all Contributors,
    ALL RIGHTS RESERVED
    [==========] Running 5 tests from 1 test case.
    [----------] Global test environment set-up.
    [----------] 5 tests from MemoryFixture
    [ RUN      ] MemoryFixture.PassTest
    [       OK ] MemoryFixture.PassTest (1 ms)
    [ RUN      ] MemoryFixture.InitalReadIsAllFs
    [       OK ] MemoryFixture.InitalReadIsAllFs (0 ms)
    [ RUN      ] MemoryFixture.WriteRead
    [       OK ] MemoryFixture.WriteRead (0 ms)
    [ RUN      ] MemoryFixture.ReadBeforeWriteLatency
    [       OK ] MemoryFixture.ReadBeforeWriteLatency (0 ms)
    [ RUN      ] MemoryFixture.ReadBeforeWriteLatencyWithThreads
    [       OK ] MemoryFixture.ReadBeforeWriteLatencyWithThreads (1 ms)
    [----------] 5 tests from MemoryFixture (115 ms total)
    
    [----------] Global test environment tear-down
    [==========] 5 tests from 1 test case ran. (158 ms total)
    [  PASSED  ] 5 tests.
    
    $
