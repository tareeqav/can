#include <map>
#include <iostream>
#include <algorithm>

#include "common.h"
#include "common_dbc.h"

namespace tareeq {
    namespace can {

/*
def clip(x, lo, hi):
    return max(lo, min(hi, x))
**/

int clip(int x, int lo, int hi)
{
    return std::max(lo, std::min(hi, x));
}

double clip(double x, double lo, double hi)
{
    return std::max(lo, std::min(hi, x));
}


class CppPacker
{
public:
    CppPacker()
    {
        packer = new CANPacker(std::string("toyota_corolla_2017_pt_generated"));
        dbc = dbc_lookup(std::string("toyota_corolla_2017_pt_generated"));

        size_t num_msgs = dbc[0].num_msgs;

        for (int i=0; i < num_msgs; i++)
        {
            const Msg msg = dbc[0].msgs[i];
        }
    }

private:
    CANPacker *packer;
    const DBC *dbc;
    std::map<std::string, std::tuple<int, int>> name_to_address_and_size;
    std::map<int, int> address_to_size;
};

    } // namespace can
} // namespace tareeq
