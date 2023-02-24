#include "src/memory_testbench.h"

MemoryTestbench::MemoryTestbench(const sc_core::sc_module_name& name)
    : SystemCTestbench(name) {
  mirror.socket.register_nb_transport_bw(this, &MemoryTestbench::NbTransportBw);
}

tlm::tlm_sync_enum MemoryTestbench::NbTransportBw(
    tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase,
    sc_core::sc_time& delay) {
  return tlm::TLM_ACCEPTED;
}
