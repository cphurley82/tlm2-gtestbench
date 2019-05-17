#include "src/memory.h"

Memory::Memory(const sc_module_name& name)
    : sc_module(name), init_data_(0xffffffffffffffff) {
  socket.register_b_transport(this, &Memory::BTransport);
}

void Memory::BTransport(tlm::tlm_generic_payload& payload, sc_time& delay) {
  auto command = payload.get_command();
  auto address = payload.get_address();
  auto data_pointer = payload.get_data_ptr();
  if (command == tlm::TLM_READ_COMMAND) {
    data_pointer = reinterpret_cast<unsigned char*>(&init_data_);
    auto data_found = sparse_memory_.find(address);
    if (data_found != sparse_memory_.end()) {
      data_pointer = reinterpret_cast<unsigned char*>(&*data_found);
    }
    payload.set_data_ptr(data_pointer);
  } else if (command == tlm::TLM_WRITE_COMMAND) {
    sparse_memory_[address] = *reinterpret_cast<uint64_t*>(data_pointer);
  }
}
