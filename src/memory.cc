#include "src/memory.h"

Memory::Memory(const sc_core::sc_module_name& name)
    : sc_module(name),
      init_data_(0xffffffffffffffff),
      work_queue_with_cb_(this, &Memory::WorkQueueCallback) {
  socket.register_b_transport(this, &Memory::BTransport);
  socket.register_nb_transport_fw(this, &Memory::NbTransportFw);
}

void Memory::BTransport(tlm::tlm_generic_payload& payload,
                        sc_core::sc_time& delay) {
  ExecuteCommand(&payload);
}

tlm::tlm_sync_enum Memory::NbTransportFw(tlm::tlm_generic_payload& payload,
                                         tlm::tlm_phase& phase,
                                         sc_core::sc_time& delay) {
  auto latency = sc_core::SC_ZERO_TIME;
  if (payload.get_command() == tlm::TLM_WRITE_COMMAND) {
    latency = sc_core::sc_time(1, sc_core::SC_SEC);
  }

  // Queue the payload until the delay has elapsed
  work_queue_with_cb_.notify(payload, phase, delay + latency);
  return tlm::TLM_ACCEPTED;
}
void Memory::WorkQueueCallback(tlm::tlm_generic_payload& payload,
                               const tlm::tlm_phase& phase) {
  ExecuteCommand(&payload);
}

void Memory::ExecuteCommand(tlm::tlm_generic_payload* payload) {
  auto command = payload->get_command();
  auto address = payload->get_address();
  auto data_pointer = payload->get_data_ptr();
  if (command == tlm::TLM_READ_COMMAND) {
    data_pointer = reinterpret_cast<unsigned char*>(&init_data_);
    auto data_found = sparse_memory_.find(address);
    if (data_found != sparse_memory_.end()) {
      data_pointer = reinterpret_cast<unsigned char*>(&*data_found);
    }
    payload->set_data_ptr(data_pointer);
  } else if (command == tlm::TLM_WRITE_COMMAND) {
    sparse_memory_[address] = *reinterpret_cast<uint64_t*>(data_pointer);
  }
}
