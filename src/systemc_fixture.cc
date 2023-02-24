#include "src/systemc_fixture.h"

void SystemCFixture::SetUp() {
  BuildTestbenchTop();
  sc_core::sc_start();
}

void SystemCFixture::TearDown() { RestartSim(); }

void SystemCFixture::RestartSim() {
  // Deconstructing the sc_curr_simcontext isn't support by SystemC
  // so we expect to leak some amount of memory here.
  sc_core::sc_curr_simcontext = new sc_core::sc_simcontext();
  sc_core::sc_default_global_context = sc_core::sc_curr_simcontext;
}

void SystemCFixture::AdvanceSimulationTime(const sc_core::sc_time& timeOffset) {
  if (timeOffset == sc_core::sc_max_time()) {
    sc_core::sc_start();
  } else {
    sc_core::sc_start(timeOffset, sc_core::SC_RUN_TO_TIME);
  }

  // Process any remaining delta cycle events
  while (sc_core::sc_pending_activity_at_current_time()) {
    sc_core::sc_start(sc_core::SC_ZERO_TIME);
  }
}
