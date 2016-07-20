// Needed for the simple_target_socket
#define SC_INCLUDE_DYNAMIC_PROCESSES

#include "systemc"
using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include "Initiator.h"
#include "Memory.h"
#include "Router.h"
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"


// *************                                 **********                   **********
// *           * init socket       target socket *        * 1               4 *        *
// * Initiator *>------------------------------->* Router *>----------------->* Memory *
// *           *                                 *        *                   *        *
// *************                                 **********                   **********

SC_MODULE(Top) {
    Initiator *initiator;
    Router<4> *router;
    Memory *memory[4];

    SC_CTOR(Top) {
        initiator = new Initiator("initiator");
        router = new Router<4>("router");
        initiator->socket.bind(router->target_socket);
        for(int i=0; i<4; i++) {
            char txt[20];
            sprintf(txt, "memory_%d", i);
            memory[i] = new Memory(txt);
            router->initiator_socket[i]->bind(memory[i]->socket);
        }
    }
};

int sc_main(int argc, char* argv[]) {
    Top top("top");
    sc_start();
    return 0;
}
