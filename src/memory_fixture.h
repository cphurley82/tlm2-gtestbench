#ifndef SRC_MEMORY_FIXTURE_H_
#define SRC_MEMORY_FIXTURE_H_

#include "src/memory.h"
#include "src/memory_testbench.h"
#include "src/systemc_fixture.h"

class MemoryFixture : public SystemCFixture {
 public:
 protected:
  void BuildTestbenchTop() override;
  MemoryTestbench& GetTestbench();

 private:
  Memory* memory_;
  MemoryTestbench* testbench_;
};

#endif  // SRC_MEMORY_FIXTURE_H_
