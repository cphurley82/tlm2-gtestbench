#include "src/systemc_fixture.h"

void SystemCFixture::SetUp() {
  BuildTestbenchTop();
  sc_start();
}

void SystemCFixture::TearDown() { RestartSim(); }

void SystemCFixture::RestartSim() {
  // Deconstructing the sc_curr_simcontext isn't support by SystemC
  // so we expect to leak some amount of memory here.
  sc_curr_simcontext = new sc_simcontext();
  sc_default_global_context = sc_curr_simcontext;
}

void SystemCFixture::AdvanceSimulationTime(const sc_time& timeOffset) {
  if (timeOffset == sc_max_time()) {
    sc_start();
  } else {
    sc_start(timeOffset, SC_RUN_TO_TIME);
  }

  // Process any remaining delta cycle events
  while (sc_pending_activity_at_current_time()) {
    sc_start(SC_ZERO_TIME);
  }
}
