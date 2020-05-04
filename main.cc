#include <iostream>

#include "utils.h"
#include "toyota_corolla_2017.h"

int main(int argc, char** argv)
{
    dbc_register(&tareeq::can::toyota_corolla_2017_pt_generated);
    tareeq::can::CppPacker* packer = new tareeq::can::CppPacker();
    return 0;
}
