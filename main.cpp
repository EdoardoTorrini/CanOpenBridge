#include <iostream>
#include "can_node_ppm.hpp"
#include "can_node_cst.hpp"

int main(int argc, char* argv[])
{
    printf("Starting program\n");
    std::string sInterface = "can0";

    try 
    {
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
        // CanOpenCST::CanNodeCST node_cst(sInterface, 0x12, vGetCST);

        scanf("INSERT: %s\n");

        node_ppm.stop();
        node_ppm.join();
        // node_cst.stop();
        
    }
    catch (CanOpenExceptionBase::CanOpenException &eErr)
    {
        printf("[ ERROR ]\n");
        printf(
            "[ MESSAGE ]: %s, [ CODE ]: %d\n", 
            eErr.getMsg().c_str(), eErr.getStatusCode()
        );
    }
    

    return 0;
}