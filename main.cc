#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <bitset>

// #include "utils.h"
// #include "can.h"

#include "packer.h"
#include "toyota_can.h"
#include "can_message.h"
#include "toyota_corolla_2017.h"

// template< typename T >
std::string int_to_hex( uint8_t i )
{
  std::stringstream stream;
  stream << std::setfill ('0') << std::setw(sizeof(i)*2) 
         << std::hex << +i;


    // std::cout << "...... " << stream.str();
  return stream.str();
}

int main(int argc, char** argv)
{

    dbc_register(&tareeq::can::toyota_corolla_2017_pt_generated);

    std::unique_ptr<tareeq::can::CANPacker> packer = tareeq::can::GetPacker(std::string("toyota_corolla_2017_pt_generated"));
    
    tareeq::can::ToyotaCAN toyota(std::move(packer));
    tareeq::can::can_message msg;

    for (size_t i=0; i < 1; i++)
    {
        msg = toyota.create_steering_command(200, 1, 50);
    }
    
    std::cout << "returned message data" << std::endl;
    for (size_t i=0; i < msg.size; i++)
    {
        std::cout << +msg.data[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "\n\nnow attempting string concatenation and convert to binary\n\n" << std::endl;

    // std::string s = "";
    // for (size_t i=0; i < msg.size; i++)
    // {
    //     s += int_to_hex(msg.data[i]);
    // }

    // std::cout << "\nstring is now " << s << std::endl;
    
    // std::stringstream ss;
    // ss << std::hex << s;
    // unsigned n;
    // ss >> n;
    // std::bitset<64> b(n);
    // // outputs "00000000000000000000000000001010"
    // std::cout << b.to_string() << std::endl;

    std::string binary;
    for (size_t i=0; i < msg.size; i++)
    {
        std::string s = int_to_hex(msg.data[i]);
        std::stringstream ss;
        ss << std::hex << s;
        unsigned n;
        ss >> n;
        std::bitset<8> b(n);
        binary += b.to_string();
    }
    std::cout << binary << std::endl;

    std::string torque_str = binary.substr(8, 16);
    std::cout << torque_str << " -- " << std::dec << std::stoi(torque_str, nullptr, 2) << std::endl;

    return 0;

}
