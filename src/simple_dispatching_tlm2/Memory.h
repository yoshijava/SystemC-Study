#ifndef MEMORY_H
#define MEMORY_H

#define MEM_SIZE 256

#include "systemc.h"
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"
#include <ctime>

SC_MODULE(Memory) {
    tlm_utils::simple_target_socket<Memory> socket;
    int memory[MEM_SIZE];

    SC_CTOR(Memory) : socket("socket") {
        socket.register_b_transport(this, &Memory::b_transport);
        socket.register_get_direct_mem_ptr(this, &Memory::get_direct_mem_ptr);

        //srand(time(NULL));
        for (int i=0 ; i<MEM_SIZE ; i++) {
            //memory[i] = 0xAA000000 | (rand() % MEM_SIZE);
            memory[i] = 0xAA000000 | i;
        }

        SC_THREAD(memoryProcess);
    }

    virtual void b_transport(tlm::tlm_generic_payload& transactions, sc_time& delay);
    virtual bool get_direct_mem_ptr(tlm::tlm_generic_payload& transactions, tlm::tlm_dmi& dmiData);
    void memoryProcess();
};

#endif
