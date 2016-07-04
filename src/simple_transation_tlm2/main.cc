// Needed for the simple_target_socket
#define SC_INCLUDE_DYNAMIC_PROCESSES

#include "systemc"
using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include "Initiator.h"
#include "Memory.h"
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"

// Target module representing a simple memory
SC_MODULE(Top) {
    Initiator *initiator;
    Memory    *memory;

    SC_CTOR(Top) {
        // Instantiate components
        initiator = new Initiator("initiator");
        memory = new Memory("memory");

        // Bind initiator socket to target socket
        initiator->socket.bind( memory->socket );
    }
};

int sc_main(int argc, char* argv[]) {
    Top top("top");
    sc_start();
    return 0;
}
