#include <gtest/gtest.h>

#include <systemc>

int sc_main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
