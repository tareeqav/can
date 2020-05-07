#pragma once

#include <iostream>

#include <cmath>
#include <memory>
#include <algorithm>

#include "packer.h"
#include "common.h"
#include "can_message.h"

namespace tareeq {
  namespace can {

class ToyotaCAN
{
private:
    
    std::unique_ptr<CANPacker> packer_;

    class Limits {
        public:
            const double_t STEER_MAX        = 1500;
            const double_t STEER_DELTA_UP   = 10;      // 1.5s time to peak torque
            const double_t STEER_DELTA_DOWN = 25;      // always lower than 45 otherwise the Rav4 faults (Prius seems ok with 50)
            const double_t STEER_ERROR_MAX  = 350;     // max delta between torque cmd and torque motor
    };

    const Limits LIMITS;

public:

    ToyotaCAN(std::unique_ptr<CANPacker>&& packer) : packer_(std::move(packer)){};

    can_message create_accel_command(
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

    can_message create_steer_command(
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

    uint8_t crc8_pedal(can_message message)
    {
        uint8_t crc = 0xFF;    // standard init value
        uint8_t poly = 0xD5;   // standard crc8: x8+x7+x6+x4+x2+1
        for (int i=(message.size-2); i >= 0; i--)
        {
            
            crc ^= message.data[i];
            std::cout << "i == " << i << " and crc = " << +crc << std::endl;
            for (int j=0; j < 8; j++)
            {
                if ((crc & 0x80) != 0) {crc = ((crc << 1) ^ poly) & 0xFF;}
                else  {crc <<= 1;}
            }
        }
        std::cout << std::endl;
        return crc;
    }

    can_message create_gas_command(double gas_amount, uint8_t idx)
    {
        std::string name("GAS_COMMAND");

        // common gas pedal msg generator
        uint8_t enable = gas_amount > 0.001;
        
        std::map<std::string, double> values = {
            {"ENABLE" , enable},
            {"COUNTER_PEDAL" , idx & 0xF}
        };

        if (enable)
        {
            values["GAS_COMMAND"] = gas_amount * 255.0;
            values["GAS_COMMAND2"] = gas_amount * 255.0;
        }

        can_message tmp = packer_->make_can_msg(name, values, -1);

        values["CHECKSUM_PEDAL"] = crc8_pedal(tmp);

        return packer_->make_can_msg(name, values, -1);
    }

   int32_t apply_steer_torque_limits(
       double apply_torque,
       double apply_torque_last,
       double motor_torque)
   {
        double max_lim = std::min(std::max(motor_torque + LIMITS.STEER_ERROR_MAX, LIMITS.STEER_ERROR_MAX), LIMITS.STEER_MAX);
        double min_lim = std::max(std::min(motor_torque - LIMITS.STEER_ERROR_MAX, -LIMITS.STEER_ERROR_MAX), -LIMITS.STEER_MAX);

        apply_torque = clip(apply_torque, min_lim, max_lim);

        if (apply_torque_last > 0)
        {
            apply_torque = clip(apply_torque,
                        std::max(apply_torque_last - LIMITS.STEER_DELTA_DOWN, -LIMITS.STEER_DELTA_UP),
                        apply_torque_last + LIMITS.STEER_DELTA_UP);
        }
        else
        {
            apply_torque = clip(apply_torque,
                        apply_torque_last - LIMITS.STEER_DELTA_UP,
                        std::min(apply_torque_last + LIMITS.STEER_DELTA_DOWN, LIMITS.STEER_DELTA_UP));
        }

        return int(std::round(float(apply_torque)));
   }    

};

  } // namespace can
} // namespace tareeq