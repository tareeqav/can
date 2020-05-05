#include <cassert>
#include <utility>
#include <algorithm>
#include <map>
#include <cmath>
#include <iostream>
#include <cstring>
#include <memory>
#include <sstream>


#include "packer.h"

#define WARN printf

namespace tareeq {
  namespace can {

// this is the same as read_u64_le, but uses uint64_t as in/out
uint64_t CANPacker::ReverseBytes(uint64_t x) {
  return ((x & 0xff00000000000000ull) >> 56) |
          ((x & 0x00ff000000000000ull) >> 40) |
          ((x & 0x0000ff0000000000ull) >> 24) |
          ((x & 0x000000ff00000000ull) >> 8) |
          ((x & 0x00000000ff000000ull) << 8) |
          ((x & 0x0000000000ff0000ull) << 24) |
          ((x & 0x000000000000ff00ull) << 40) |
          ((x & 0x00000000000000ffull) << 56);
}

uint64_t CANPacker::set_value(uint64_t ret, Signal sig, int64_t ival){
  int shift = sig.is_little_endian? sig.b1 : sig.bo;
  uint64_t mask = ((1ULL << sig.b2)-1) << shift;
  uint64_t dat = (ival & ((1ULL << sig.b2)-1)) << shift;

  if (sig.is_little_endian) {
    dat = ReverseBytes(dat);
    mask = ReverseBytes(mask);
  }

  ret &= ~mask;
  ret |= dat;
  return ret;
}

CANPacker::CANPacker(const std::string& dbc_name) {
  dbc = dbc_lookup(dbc_name);
  assert(dbc);
  for (int i=0; i<dbc->num_msgs; i++) {
    const Msg* msg = &dbc->msgs[i];
    message_lookup[msg->address] = *msg;
    for (int j=0; j<msg->num_sigs; j++) {
      const Signal* sig = &msg->sigs[j];
      signal_lookup[std::make_pair(msg->address, std::string(sig->name))] = *sig;
    }
  }
  init_crc_lookup_tables();

  size_t num_msgs = dbc[0].num_msgs;
  for (size_t i=0; i < num_msgs; i++)
  {
    Msg msg = dbc[0].msgs[i];
    name_to_address_and_size[msg.name] = std::make_pair(msg.address, msg.size);
    address_to_size[msg.address] = msg.size;
  }

}

uint64_t CANPacker::pack(uint32_t address, const std::vector<SignalPackValue> &signals, int counter) {
  uint64_t ret = 0;

  for (const auto& sigval : signals) {
    std::string name = std::string(sigval.name);
    double value = sigval.value;

    auto sig_it = signal_lookup.find(std::make_pair(address, name));
    if (sig_it == signal_lookup.end()) {
      WARN("undefined signal %s - %d\n", name.c_str(), address);
      continue;
    }
    auto sig = sig_it->second;

    int64_t ival = (int64_t)(round((value - sig.offset) / sig.factor));
    if (ival < 0) {
      ival = (1ULL << sig.b2) + ival;
    }

    ret = set_value(ret, sig, ival);
  }

  if (counter >= 0){
    auto sig_it = signal_lookup.find(std::make_pair(address, "COUNTER"));
    if (sig_it == signal_lookup.end()) {
      WARN("COUNTER not defined\n");
      return ret;
    }
    auto sig = sig_it->second;

    if ((sig.type != SignalType::HONDA_COUNTER) && (sig.type != SignalType::VOLKSWAGEN_COUNTER)) {
      WARN("COUNTER signal type not valid\n");
    }

    ret = set_value(ret, sig, counter);
  }

  auto sig_it_checksum = signal_lookup.find(std::make_pair(address, "CHECKSUM"));
  if (sig_it_checksum != signal_lookup.end()) {
    auto sig = sig_it_checksum->second;
    unsigned int chksm = toyota_checksum(address, ret, message_lookup[address].size);
    ret = set_value(ret, sig, chksm);
  }

  return ret;
}

CANPacker::can_pack CANPacker::make_can_msg(std::string &name, std::map<std::string, double> &values, int counter)
{

  std::vector<SignalPackValue> signals;

  std::map<std::string, double>::iterator it = values.begin();
  while (it != values.end())
  {
    SignalPackValue signal;

    std::string key = it->first;
    double value = it->second;
    
    signal.name = new char[key.length()+1];
    std::strcpy((char *)signal.name, key.c_str());
    signal.value = value;

    //add to list
    signals.push_back(signal);

    it++;
  }

  std::pair<int, int> address_and_size = name_to_address_and_size[name];

  uint64_t val = pack(address_and_size.first, signals, -1);
  std::cout << "generated value " << val << " and as an array is:" << std::endl;
  val = ReverseBytes(val);

  std::cout << std::hex;
  std::cout << "reversed bytes value " << val << " and as an array is:" << std::endl;
  for (int i=0; i < address_and_size.second; i++) {
    std::cout << +(*(((uint8_t*)&val)+i)) << " " ;
  }

  std::cout << std::endl;
  can_pack msg;
  msg.data = new uint8_t[address_and_size.second];
  std::memcpy(msg.data, &val, address_and_size.second);

  msg.size = address_and_size.second;
  msg.bus = 0;

  return msg;

}

CANPacker::can_pack CANPacker::create_steer_command(
  double steer_torque_value, 
  double steer_request_on_off,
  double counter
)
{
  std::string name("STEERING_LKA");

  std::map<std::string, double> values = {
    {"STEER_REQUEST" , steer_request_on_off},
    {"STEER_TORQUE_CMD" , steer_torque_value},
    {"SET_ME_1" , 1.0},
    {"COUNTER" , counter},
  };

  return make_can_msg(name, values, counter);
}

  } // namespace can
} // namespace tareeq
