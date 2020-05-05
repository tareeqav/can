#include <iostream>

#include "utils.h"
#include "packer.h"
#include "toyota_corolla_2017.h"

int main(int argc, char** argv)
{
    dbc_register(&tareeq::can::toyota_corolla_2017_pt_generated);
    // tareeq::can::CppPacker* packer = new tareeq::can::CppPacker();
    tareeq::can::CANPacker* packer = new tareeq::can::CANPacker(std::string("toyota_corolla_2017_pt_generated"));
    const tareeq::can::CANPacker::can_msg msg = packer->create_steer_command(2.0, 1.0, 11.0);
    std::cout << "messsage address is " << msg.address << " and message size is " << msg.size << std::endl;
    std::cout << "message value is " << msg.val << std::endl;
    return 0;
}
