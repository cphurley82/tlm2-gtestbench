#ifndef SRC_MEMORY_H_
#define SRC_MEMORY_H_

#include <map>

#include "ext/systemc/src/systemc.h"
#include "ext/systemc/src/tlm.h"
#include "ext/systemc/src/tlm_utils/peq_with_cb_and_phase.h"
#include "ext/systemc/src/tlm_utils/simple_target_socket.h"

class Memory : public sc_core::sc_module {
 public:
  explicit Memory(const sc_module_name& name);

  tlm_utils::simple_target_socket<Memory> socket;

 private:
  void BTransport(tlm::tlm_generic_payload& payload, sc_time& delay);

  // NOLINTNEXTLINE
  tlm::tlm_sync_enum NbTransportFw(tlm::tlm_generic_payload& payload,
                                   // NOLINTNEXTLINE
                                   tlm::tlm_phase& phase, sc_time& delay);

  // NOLINTNEXTLINE
  void WorkQueueCallback(tlm::tlm_generic_payload& payload,
                         const tlm::tlm_phase& phase);

  uint64_t init_data_;
  std::map<uint64_t, uint64_t> sparse_memory_;
  tlm_utils::peq_with_cb_and_phase<Memory> work_queue_with_cb_;
  void ExecuteCommand(tlm::tlm_generic_payload* payload);
};

#endif  // SRC_MEMORY_H_
