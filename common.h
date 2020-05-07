#pragma once

#include <map>
#include <vector>

#include "dbc.h"

namespace tareeq {
  namespace can {

    
#define MAX_BAD_COUNTER 5

// Helper functions
unsigned int toyota_checksum(unsigned int address, uint64_t d, int l);
void init_crc_lookup_tables();
unsigned int pedal_checksum(uint64_t d, int l);
uint64_t read_u64_be(const uint8_t* v);
uint64_t read_u64_le(const uint8_t* v);
int clip(int x, int lo, int hi);
double clip(double x, double lo, double hi);

/** 
// class MessageState {
// public:
//   uint32_t address;
//   unsigned int size;

//   std::vector<Signal> parse_sigs;
//   std::vector<double> vals;

//   uint16_t ts;
//   uint64_t seen;
//   uint64_t check_threshold;

//   uint8_t counter;
//   uint8_t counter_fail;

//   bool parse(uint64_t sec, uint16_t ts_, uint8_t * dat);
//   bool update_counter_generic(int64_t v, int cnt_size);
// };

// class CANParser {
// private:
//   const int bus;

//   const DBC *dbc = NULL;
//   std::map<uint32_t, MessageState> message_states;

// public:
//   bool can_valid = false;
//   uint64_t last_sec = 0;

//   CANParser(int abus, const std::string& dbc_name,
//             const std::vector<MessageParseOptions> &options,
//             const std::vector<SignalParseOptions> &sigoptions);
  
//   // void UpdateCans(uint64_t sec, const capnp::List<cereal::CanData>::Reader& cans);
//   void UpdateValid(uint64_t sec);
//   void update_string(std::string data, bool sendcan);
//   std::vector<SignalValue> query_latest();

// };

*/

  } // namespace can
} // namespace tareeq
