#include "src/memory.h"

Memory::Memory(const sc_module_name& name) : sc_module(name) {
  socket.register_b_transport(this, &Memory::BTransport);
}

void Memory::BTransport(tlm::tlm_generic_payload& payload, sc_time& delay) {}
