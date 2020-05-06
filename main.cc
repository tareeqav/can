#include <iostream>

#include "utils.h"
// #include "can.h"
#include "packer.h"
#include "toyota_corolla_2017.h"

int main(int argc, char** argv)
{

    dbc_register(&tareeq::can::toyota_corolla_2017_pt_generated);
    tareeq::can::CANPacker* packer = new tareeq::can::CANPacker(std::string("toyota_corolla_2017_pt_generated"));
    tareeq::can::can_message msg;
    for (int i=0; i < 1; i++)
    {
        msg = packer->create_accel_command(0, 1, 0, 1);
    }
    
    std::cout << "returned message data" << std::endl;
    for (int i=0; i < msg.size; i++)
    {
        std::cout << +msg.data[i] << " ";
    }
    std::cout << std::endl;
    return 0;

}
