#pragma once

#include <memory>

#include "packer.h"
#include "can_message.h"

namespace tareeq {
  namespace can {

class ToyotaCAN
{
public:

    ToyotaCAN(std::unique_ptr<CANPacker>&& packer) : packer_(std::move(packer)){};

    tareeq::can::can_message create_accel_command(
        double accel_value, 
        double pcm_cancel,
        double standstill_req,
        double lead)
    {
        std::string name("ACC_CONTROL");

        std::map<std::string, double> values = {
            {"ACCEL_CMD", accel_value},
            {"SET_ME_X01", 1},
            {"DISTANCE", 0},
            {"MINI_CAR", lead},
            {"SET_ME_X3", 3},
            {"SET_ME_1", 1},
            {"RELEASE_STANDSTILL", not standstill_req},
            {"CANCEL_REQ", pcm_cancel}
        };

        return packer_->make_can_msg(name, values, -1);
    };

    tareeq::can::can_message create_steer_command(
        double steer_torque_value, 
        double steer_request_on_off,
        double counter)
    {
        std::string name("STEERING_LKA");

        std::map<std::string, double> values = {
            {"STEER_REQUEST" , steer_request_on_off},
            {"STEER_TORQUE_CMD" , steer_torque_value},
            {"SET_ME_1" , 1.0},
            {"COUNTER" , counter},
        };

        return packer_->make_can_msg(name, values, (int) counter);
    };

private:
    std::unique_ptr<CANPacker> packer_;

};

  } // namespace can
} // namespace tareeq