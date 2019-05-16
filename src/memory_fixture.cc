#include "src/memory_fixture.h"

void MemoryFixture::BuildTestbenchTop() {
  memory_ = new Memory("Memory");
  testbench_ = new MemoryTestbench("MemoryTestbench");
  testbench_->bind(memory_);
}
