#ifndef SRC_MEMORY_H_
#define SRC_MEMORY_H_

#include <tlm_utils/peq_with_cb_and_phase.h>
#include <tlm_utils/simple_target_socket.h>

#include <map>
#include <systemc>
#include <tlm>

class Memory : public sc_core::sc_module {
 public:
  explicit Memory(const sc_core::sc_module_name& name);

  tlm_utils::simple_target_socket<Memory> socket;

 private:
  void BTransport(tlm::tlm_generic_payload& payload, sc_core::sc_time& delay);

  // NOLINTNEXTLINE
  tlm::tlm_sync_enum NbTransportFw(tlm::tlm_generic_payload& payload,
                                   // NOLINTNEXTLINE
                                   tlm::tlm_phase& phase,
                                   sc_core::sc_time& delay);

  // NOLINTNEXTLINE
  void WorkQueueCallback(tlm::tlm_generic_payload& payload,
                         const tlm::tlm_phase& phase);

  uint64_t init_data_;
  std::map<uint64_t, uint64_t> sparse_memory_;
  tlm_utils::peq_with_cb_and_phase<Memory> work_queue_with_cb_;
  void ExecuteCommand(tlm::tlm_generic_payload* payload);
};

#endif  // SRC_MEMORY_H_
