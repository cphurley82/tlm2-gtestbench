#include <memory>

#include "src/memory_fixture.h"

TEST_F(MemoryFixture, PassTest) { EXPECT_TRUE(true); }

TEST_F(MemoryFixture, InitalReadIsAllFs) {
  auto payload = std::make_shared<tlm::tlm_generic_payload>();
  auto delay = sc_time(1, SC_SEC);
  payload->set_command(tlm::TLM_READ_COMMAND);

  GetTestbench().mirror.socket->b_transport(*payload, delay);
  EXPECT_EQ(*reinterpret_cast<uint64_t*>(payload->get_data_ptr()),
            0xffffffffffffffff);
}

TEST_F(MemoryFixture, WriteRead) {
  const auto num_addresses = int{10};

  auto payload = std::make_shared<tlm::tlm_generic_payload>();
  auto delay = sc_time(1, SC_SEC);

  for (auto address = uint64_t{0}; address < num_addresses; address++) {
    payload->set_command(tlm::TLM_WRITE_COMMAND);
    payload->set_address(address);
    payload->set_data_ptr(reinterpret_cast<unsigned char*>(&address));
    GetTestbench().mirror.socket->b_transport(*payload, delay);

    payload->set_command(tlm::TLM_READ_COMMAND);
    GetTestbench().mirror.socket->b_transport(*payload, delay);
    EXPECT_EQ(*reinterpret_cast<uint64_t*>(payload->get_data_ptr()), address);
  }
}

// When we write to an address and then read from the same address before
// the write latency has elapsed we should get the data back from before
// the write was issued.
TEST_F(MemoryFixture, ReadBeforeWriteLatency) {
  auto address = uint64_t{0};
  auto write_data = address;
  auto phase = tlm::tlm_phase(tlm::BEGIN_REQ);
  auto delay = sc_time(1, SC_PS);

  // Issue write command.
  auto write_payload = std::make_shared<tlm::tlm_generic_payload>();
  write_payload->set_command(tlm::TLM_WRITE_COMMAND);
  write_payload->set_address(address);
  write_payload->set_data_ptr(reinterpret_cast<unsigned char*>(&write_data));

  sc_spawn([&]() {
    GetTestbench().mirror.socket->nb_transport_fw(*write_payload, phase, delay);
  });
  AdvanceSimulationTime(sc_time(1, SC_PS));  // Less than write latency

  // Issue read command before write latency has elapsed.
  auto read_payload = std::make_shared<tlm::tlm_generic_payload>();
  read_payload->set_command(tlm::TLM_READ_COMMAND);
  read_payload->set_address(address);

  sc_spawn([&]() {
    GetTestbench().mirror.socket->nb_transport_fw(*read_payload, phase, delay);
  });
  AdvanceSimulationTime();

  // Expect the data read to be the initial value, not the data written.
  auto read_data = *reinterpret_cast<uint64_t*>(read_payload->get_data_ptr());
  EXPECT_EQ(read_data, 0xffffffffffffffff);
}
