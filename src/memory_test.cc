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
