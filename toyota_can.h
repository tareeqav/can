#pragma once

#include <memory>

#include "packer.h"
#include "can_message.h"

namespace tareeq {
  namespace can {

class ToyotaCAN
{
    // /**
// def create_accel_command(packer, accel, pcm_cancel, standstill_req, lead):
//   # TODO: find the exact canceling bit that does not create a chime
//   values = {
//     "ACCEL_CMD": accel,
//     "SET_ME_X01": 1,
//     "DISTANCE": 0,
//     "MINI_CAR": lead,
//     "SET_ME_X3": 3,
//     "SET_ME_1": 1,
//     "RELEASE_STANDSTILL": not standstill_req,
//     "CANCEL_REQ": pcm_cancel,
//   }
//   return packer.make_can_msg("ACC_CONTROL", 2, values)

// */

public:

    ToyotaCAN(const CANPacker &packer) : packer_(packer){};

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

        return packer_.make_can_msg(name, values, -1);
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

        return packer_.make_can_msg(name, values, (int) counter);
    };

private:
    const CANPacker packer_;

// /**
//  * def create_gas_command(packer, gas_amount, idx):
//     # Common gas pedal msg generator
//     enable = gas_amount > 0.001

//     values = {
//         "ENABLE": enable,
//         "COUNTER_PEDAL": idx & 0xF,
//     }

//     if enable:
//         values["GAS_COMMAND"] = gas_amount * 255.
//         values["GAS_COMMAND2"] = gas_amount * 255.

//     dat = packer.make_can_msg("GAS_COMMAND", 2, values)[2]

//     checksum = crc8_pedal(dat[:-1])
//     values["CHECKSUM_PEDAL"] = checksum

//     return packer.make_can_msg("GAS_COMMAND", 2, values)


};

  } // namespace can
} // namespace tareeq