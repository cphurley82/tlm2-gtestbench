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
  auto delay = SC_ZERO_TIME;

  // Issue write command.
  auto write_payload = std::make_shared<tlm::tlm_generic_payload>();
  write_payload->set_command(tlm::TLM_WRITE_COMMAND);
  write_payload->set_address(address);
  write_payload->set_data_ptr(reinterpret_cast<unsigned char*>(&write_data));
  GetTestbench().mirror.socket->nb_transport_fw(*write_payload, phase, delay);
  AdvanceSimulationTime(sc_time(1, SC_PS));  // Less than write latency

  // Issue read command before write latency has elapsed.
  auto pre_read_payload = std::make_shared<tlm::tlm_generic_payload>();
  pre_read_payload->set_command(tlm::TLM_READ_COMMAND);
  pre_read_payload->set_address(address);
  GetTestbench().mirror.socket->nb_transport_fw(*pre_read_payload, phase,
                                                delay);
  AdvanceSimulationTime();

  // Expect the pre-read data read to be the initial value, not the data
  // written.
  auto pre_read_data =
      *reinterpret_cast<uint64_t*>(pre_read_payload->get_data_ptr());
  EXPECT_EQ(pre_read_data, 0xffffffffffffffff);

  // Issue read command after write latency has elapsed.
  auto post_read_payload = std::make_shared<tlm::tlm_generic_payload>();
  post_read_payload->set_command(tlm::TLM_READ_COMMAND);
  post_read_payload->set_address(address);
  GetTestbench().mirror.socket->nb_transport_fw(*post_read_payload, phase,
                                                delay);
  AdvanceSimulationTime();

  // Expect the post-read data read to be the data written.
  auto post_read_data =
      *reinterpret_cast<uint64_t*>(post_read_payload->get_data_ptr());
  EXPECT_EQ(post_read_data, write_data);
}

TEST_F(MemoryFixture, ReadBeforeWriteLatencyWithThreads) {
  const auto address = uint64_t{0};
  auto write_data = address;
  auto phase = tlm::tlm_phase(tlm::BEGIN_REQ);
  auto delay = SC_ZERO_TIME;

  // Issue write command.
  // Note: Payloads must be created outside of threads so they do not get
  // destroyed before they are needed.
  auto write_payload = std::make_shared<tlm::tlm_generic_payload>();
  sc_spawn([&] {
    write_payload->set_command(tlm::TLM_WRITE_COMMAND);
    write_payload->set_address(address);
    write_payload->set_data_ptr(reinterpret_cast<unsigned char*>(&write_data));
    GetTestbench().mirror.socket->nb_transport_fw(*write_payload, phase, delay);
  });

  // Issue read command before write latency has elapsed.
  auto pre_read_payload = std::make_shared<tlm::tlm_generic_payload>();
  sc_spawn([&] {
    wait(sc_time(1, SC_PS));  // Wait a short time
    pre_read_payload->set_command(tlm::TLM_READ_COMMAND);
    pre_read_payload->set_address(address);
    GetTestbench().mirror.socket->nb_transport_fw(*pre_read_payload, phase,
                                                  delay);
  });

  // Issue read command after write latency has elapsed.
  auto post_read_payload = std::make_shared<tlm::tlm_generic_payload>();
  sc_spawn([&] {
    wait(sc_time(2, SC_SEC));  // Wait a long time
    post_read_payload->set_command(tlm::TLM_READ_COMMAND);
    post_read_payload->set_address(address);
    GetTestbench().mirror.socket->nb_transport_fw(*post_read_payload, phase,
                                                  delay);
  });

  AdvanceSimulationTime();

  // Expect the pre-read data read to be the initial value, not the data
  // written.
  auto pre_read_data =
      *reinterpret_cast<uint64_t*>(pre_read_payload->get_data_ptr());
  EXPECT_EQ(pre_read_data, 0xffffffffffffffff);

  // Expect the post-read data read to be the data written.
  auto post_read_data =
      *reinterpret_cast<uint64_t*>(post_read_payload->get_data_ptr());
  EXPECT_EQ(post_read_data, write_data);
}
