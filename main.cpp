#include <iostream>
#include "can_node_ppm.hpp"
#include "can_node_cst.hpp"

int main(int argc, char **argv)
{
    std::string sInterface = "can0";

    // --------------------------- PPM --------------------------------
    std::vector<int> vGetPPM = {
        CanOpenHeaderUtils::PPM_READ_STATUSWORD_TARGET_REACHED,
    };
    CanOpenPPM::CanNodePPM node_ppm(sInterface, 0x01, vGetPPM);

    // --------------------------- CST --------------------------------
    std::vector<int> vGetCST = {
        CanOpenHeaderUtils::CST_READ_STATUSWORD,
        CanOpenHeaderUtils::CST_READ_POSITION,
        CanOpenHeaderUtils::CST_READ_VELOCITY_ACTUAL_VALUE_AVERAGED,
        CanOpenHeaderUtils::CST_READ_TORQUE_ACTUAL_VALUE
    };
    CanOpenCST::CanNodeCST node_cst(sInterface, 0x01, vGetCST);

    return 0;
}