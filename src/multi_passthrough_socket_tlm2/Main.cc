// *****************************************************************************************
// Top-level module instantiates 2 initiators, a bus, and 4 memories
// *****************************************************************************************
#include "systemc"
#include "Initiator.h"
#include "Bus.h"
#include "Target.h"

using namespace sc_core;
using namespace sc_dt;

#define SC_INCLUDE_DYNAMIC_PROCESSES

SC_MODULE(Top)
{
    Initiator* initiator[4];
    Bus* bus;
    Target* target[4];

    SC_CTOR(Top)
    {
        bus = new Bus("bus");
        for (int i=0 ; i<4 ; i++) {
            char txt[20];
            sprintf(txt, "initiator_%d", i);
            initiator[i] = new Initiator(txt);
            initiator[i]->socket.bind(bus->targetSocket);
        }

        for (int i=0 ; i<4 ; i++) {
            char txt[20];
            sprintf(txt, "target_%d", i);
            target[i] = new Target(txt);
            bus->initiatorSocket.bind(target[i]->socket);
        }
    }
};

int sc_main( int argc, char* argv[])
{
    Top top("top");
    sc_start();
    return 0;
}