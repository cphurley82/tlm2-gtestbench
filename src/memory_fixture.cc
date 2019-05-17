#include "src/memory_fixture.h"

void MemoryFixture::BuildTestbenchTop() {
  memory_ = new Memory("Memory");
  testbench_ = new MemoryTestbench("MemoryTestbench");
  testbench_->mirror.socket.bind(memory_->socket);
}

MemoryTestbench& MemoryFixture::GetTestbench() { return *testbench_; }
