#pragma once

#include <memory>
#include <utility>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <cstring>
#include <map>
#include <sstream>

#include "can.h"

namespace tareeq {
    namespace can {

class CANPacker : public CANBase {

public:
  
  CANPacker(const std::string& dbc_name);
  ~CANPacker();

  tareeq::can::can_message create_accel_command(double accel_value, double pcm_cancel, double standstill_req,double lead);
  tareeq::can::can_message create_steer_command(double steer_torque_cmd, double steer_request_on, double counter);
  tareeq::can::can_message make_can_msg(std::string &name, std::map<std::string, double> &values, int counter);

private:

  uint64_t set_value(uint64_t ret, Signal sig, int64_t ival);
  uint64_t pack(uint32_t address, const std::vector<SignalPackValue> &signals, int counter);
  
};

    } // namespace can
} // namespace tareeq
