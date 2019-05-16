#include "ext/googletest/googletest/include/gtest/gtest.h"
#include "ext/systemc/src/systemc.h"

int sc_main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
