// *****************************************************************************************
// Top-level module instantiates 2 initiators, a bus, and 4 memories
// *****************************************************************************************
#include "systemc"
#include "Global.h"
#include "Initiator1.h"
#include "Initiator2.h"
#include "Bus.h"
#include "Memory.h"
using namespace sc_core;
using namespace sc_dt;

#define SC_INCLUDE_DYNAMIC_PROCESSES

SC_MODULE(Top)
{
    Initiator1* initiator1;
    Initiator2* initiator2;
    Bus* bus;
    Memory* memory[4];

    SC_CTOR(Top)
    {
        initiator1 = new Initiator1("initiator1");
        initiator2 = new Initiator2("initiator2");
        bus        = new Bus(2, 4);

        initiator1->socket.bind( *(bus->targetSocket[0]) );
        initiator2->socket.bind( *(bus->targetSocket[1]) );

        for (int i = 0; i < 4; i++)
        {
            char txt[20];
            sprintf(txt, "memory_%d", i);
            memory[i] = new Memory(txt);

            ( *(bus->initiatorSocket[i]) ).bind( memory[i]->socket );
        }
    }
};

int sc_main( int argc, char* argv[])
{
    Top top("top");
    sc_start();
    return 0;
}