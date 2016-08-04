// Shows the non-blocking transport interface with the generic payload and simple sockets
// Shows nb_transport used with the forward and backward paths
// Both components are able to accept transactions on the return path,
// although neither component actually uses the return path (TLM_UPDATED)

// Shows the Approximately Timed coding style
// Models processing delay of initiator, latency of target, and request and response accept delays
// Uses payload event queues to manage both timing annotations and internal delays

// Shows the BEGIN_REQ exclusion rule at the initiator and BEGIN_RESP exclusion rule at the target
// In this example, the target allows two pipelined transactions in-flight

// Shows an explicit memory manager and reference counting

// No use of temporal decoupling, DMI or debug transport
// Nominal use of the blocking transport interface just to show the simple socket b/nb adapter

// Needed for the simple_target_socket
#define SC_INCLUDE_DYNAMIC_PROCESSES

#include "systemc"
using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/peq_with_cb_and_phase.h"
#include "Initiator.h"
#include "Target.h"
#define DEBUG
SC_MODULE(Top) {
    Initiator *initiator;
    Target    *target;

    SC_CTOR(Top) {
        // Instantiate components
        initiator = new Initiator("initiator");
        target    = new Target   ("target");

        // One initiator is bound directly to one target with no intervening bus

        // Bind initiator socket to target socket
        initiator->socket.bind(target->socket);
    }
};

int sc_main(int argc, char* argv[]) {
    Top top("top");
    sc_start();
    return 0;
}
