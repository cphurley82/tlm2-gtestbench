#ifndef SRC_SYSTEMC_TESTBENCH_H_
#define SRC_SYSTEMC_TESTBENCH_H_

#include "ext/systemc/src/systemc.h"

class SystemCTestbench : public sc_core::sc_module {
 public:
  explicit SystemCTestbench(const sc_module_name &nm);
};

#endif  // SRC_SYSTEMC_TESTBENCH_H_
