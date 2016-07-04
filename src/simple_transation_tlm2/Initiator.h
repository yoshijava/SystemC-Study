#ifndef Initiator_H
#define Initiator_H

#include "systemc"
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"

using namespace sc_core;
using namespace sc_dt;
using namespace std;
using namespace tlm_utils;

// Initiator module generating generic payload transactions

SC_MODULE(Initiator) {
    // TLM-2 socket, defaults to 32-bits wide, base protocol
    simple_initiator_socket<Initiator> socket;
    // Internal data buffer used by initiator with generic payload
    int data;

    // Construct and name socket
    SC_CTOR(Initiator) : socket("socket") {
        SC_THREAD(thread_process);
    }

    void thread_process();

};

#endif
