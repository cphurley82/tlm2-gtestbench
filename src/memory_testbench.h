#ifndef SRC_MEMORY_TESTBENCH_H_
#define SRC_MEMORY_TESTBENCH_H_

#include <tlm_utils/simple_initiator_socket.h>

#include "src/memory.h"
#include "src/systemc_testbench.h"

class MemoryTestbench : public SystemCTestbench {
 public:
  explicit MemoryTestbench(const sc_core::sc_module_name& name);

  // Testbench interface mirrors the device under test.
  struct {
    tlm_utils::simple_initiator_socket<MemoryTestbench> socket;
  } mirror;

 private:
  // NOLINTNEXTLINE
  virtual tlm::tlm_sync_enum NbTransportBw(tlm::tlm_generic_payload& trans,
                                           // NOLINTNEXTLINE
                                           tlm::tlm_phase& phase,
                                           // NOLINTNEXTLINE
                                           sc_core::sc_time& delay);
};

#endif  // SRC_MEMORY_TESTBENCH_H_
