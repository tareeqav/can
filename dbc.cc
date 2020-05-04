#include <vector>
#include <iostream>
#include "common_dbc.h"

namespace tareeq {
  namespace can {

std::vector<const DBC*>& get_dbcs() {
  static std::vector<const DBC*> vec;
  return vec;
}

const DBC* dbc_lookup(const std::string& dbc_name) {
  std::cout << "in dbc lookup with dbc_name " << dbc_name << std::endl;
  for (const auto& dbci : get_dbcs()) {
    std::cout << "got dbci " << dbci << std::endl;
    if (dbc_name == dbci->name) {
      return dbci;
    }
  }
  return NULL;
}

void dbc_register(const DBC* dbc) {
  get_dbcs().push_back(dbc);
}

// extern "C" {
  const DBC* dbc_lookup(const char* dbc_name) {
    return dbc_lookup(std::string(dbc_name));
  }
// }

  }
}

