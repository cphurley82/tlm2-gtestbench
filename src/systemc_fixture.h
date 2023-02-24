#ifndef SRC_SYSTEMC_FIXTURE_H_
#define SRC_SYSTEMC_FIXTURE_H_

// Base class for all module test fixtures.

#include <gtest/gtest.h>

#include <systemc>
#include <tlm>

class SystemCFixture : public testing::Test {
 protected:
  void SetUp() override;     // Build the top level and start simulation.
  void TearDown() override;  // Restart the simulator at the end.
  virtual void BuildTestbenchTop() = 0;  // Construct and bind modules
  static void AdvanceSimulationTime(
      const sc_core::sc_time& timeOffset = sc_core::sc_max_time());

 private:
  static void RestartSim();
};

#endif  // SRC_SYSTEMC_FIXTURE_H_
