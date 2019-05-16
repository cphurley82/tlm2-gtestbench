#include "src/memory_testbench.h"

MemoryTestbench::MemoryTestbench(const sc_core::sc_module_name& name)
    : SystemCTestbench(name) {}

void MemoryTestbench::bind(Memory* target) {
  target->socket.bind(mirror.socket);
}
