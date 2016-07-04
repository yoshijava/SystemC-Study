#ifndef Memory_H
#define Memory_H

#include "systemc"
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"

using namespace sc_core;
using namespace sc_dt;
using namespace std;
using namespace tlm_utils;

SC_MODULE(Memory) {
    // TLM-2 socket, defaults to 32-bits wide, base protocol
    simple_target_socket<Memory> socket;
    enum {
        SIZE = 256
    };
    int mem[SIZE];

    SC_CTOR(Memory) : socket("socket") {
        // Register callback for incoming b_transport interface method call
        socket.register_b_transport(this, &Memory::b_transport);

        // Initialize memory with random data
        for (int i = 0; i < SIZE; i++) {
            mem[i] = 0xAA000000 | (rand() % 256);
        }
    }
    void b_transport(tlm::tlm_generic_payload& trans, sc_time& delay);
};

#endif
