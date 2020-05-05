#pragma once

#include "common.h"

namespace tareeq {
    namespace can {

class CANPacker {

public:
  struct can_pack
  {
      uint32_t          address;
      uint8_t*          data;
      uint32_t          bus;
      uint32_t          size;
  };
  
  CANPacker(const std::string& dbc_name);
  CANPacker::can_pack create_steer_command(double steer_torque_cmd, double steer_request_on, double counter);

private:
  const DBC *dbc = NULL;
  std::map<std::pair<uint32_t, std::string>, Signal> signal_lookup;
  std::map<uint32_t, Msg> message_lookup;
  std::map<std::string, std::pair<uint32_t, uint32_t>> name_to_address_and_size;
  std::map<uint32_t, uint32_t> address_to_size;

  uint64_t pre_pack(uint32_t address, const std::vector<SignalPackValue> &signals, int counter);
  uint64_t ReverseBytes(uint64_t x);
  uint64_t set_value(uint64_t ret, Signal sig, int64_t ival);
  uint64_t pack(uint32_t address, const std::vector<SignalPackValue> &signals, int counter);
  can_pack make_can_msg(std::string &name, std::map<std::string, double> &values, int counter);
};

    } // namespace can
} // namespace tareeq
