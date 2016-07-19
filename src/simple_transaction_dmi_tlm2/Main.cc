#define SC_INCLUDE_DYNAMIC_PROCESSES

#include "systemc.h"
#include "Initiator.h"
#include "Memory.h"

bool intentionalError = false;

SC_MODULE(Top) {
    Initiator* initiator;
    Memory* memory;

    SC_CTOR(Top) {
        initiator = new Initiator("master", intentionalError);
        memory = new Memory("memory");

        initiator->socket.bind(memory->socket);
    }
};

int sc_main(int argc, char* argv[]) {
    if (argc >= 2) {
        if (atoi(argv[1]) != 0){
            intentionalError = true;
        }
    }

    Top top("top");
    sc_start();
    return 0;
}