#include <iostream>

#include "src/can_node.cpp"

int main(int argc, char **argv)
{
    CanNode node1 = CanNode("vcan0", 0x01);
    node1.set_operational_mode();

    sleep(10);

    return 0;
}