#include <iostream>
#include "can_node_ppm.hpp"

int main(int argc, char **argv)
{
    std::vector<int> vGetPPM = {
        CanOpenHeaderUtils::PPM_READ_STATUSWORD_TARGET_REACHED,
    };
    std::string sInterface = "can0";

    CanOpenPPM::CanNodePPM node(sInterface, 0x01, vGetPPM);
    return 0;
}