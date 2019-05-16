#ifndef SRC_MEMORY_H_
#define SRC_MEMORY_H_

#include "ext/systemc/src/systemc.h"
#include "ext/systemc/src/tlm.h"
#include "ext/systemc/src/tlm_utils/simple_target_socket.h"

class Memory : public sc_core::sc_module {
 public:
  explicit Memory(const sc_module_name& name);

  tlm_utils::simple_target_socket<Memory> socket;

 private:
  void BTransport(tlm::tlm_generic_payload& payload, sc_time& delay);
};

#endif  // SRC_MEMORY_H_
