#include "src/systemc_testbench.h"

SystemCTestbench::SystemCTestbench(const sc_core::sc_module_name &nm)
    : sc_module(nm) {
  // Prevents an error when a test is run that does not start the simulator.
  sc_core::sc_report_handler::set_actions(sc_core::SC_ID_NO_SC_START_ACTIVITY_,
                                          sc_core::SC_DO_NOTHING);
}
