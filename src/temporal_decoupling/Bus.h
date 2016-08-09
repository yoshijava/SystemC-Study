#ifndef BUS_H
#define BUS_H

#include "systemc.h"
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"

SC_MODULE(Bus) {
    simple_target_socket_tagged<Bus>** targetSocket;
    simple_initiator_socket_tagged<Bus>** initiatorSocket;
    int initiatorNum;
    int targetNum;

    Bus(int initNum, int targNum) : sc_module("Bus") {
        initiatorNum = initNum;
        targetNum = targNum;
        targetSocket = (simple_target_socket_tagged<Bus>**) malloc (sizeof(simple_target_socket_tagged<Bus>*) * initiatorNum);
        initiatorSocket = (simple_initiator_socket_tagged<Bus>**) malloc (sizeof(simple_initiator_socket_tagged<Bus>*) * targetNum);
        for (int i=0 ; i<initiatorNum ; i++) {
            char txt[20];
            sprintf(txt, "targetSocket_%d", i);
            targetSocket[i] = new simple_target_socket_tagged<Bus>(txt);
            targetSocket[i]->register_b_transport       (this, &Bus::b_transport, i);
            //targetSocket[i]->register_nb_transport_fw   (this, &Bus::nb_transport_fw, i);
            targetSocket[i]->register_get_direct_mem_ptr(this, &Bus::get_direct_mem_ptr, i);
        }
        for (int i=0 ; i<targetNum ; i++) {
            char txt[20];
            sprintf(txt, "initSocket_%d", i);
            initiatorSocket[i] = new simple_initiator_socket_tagged<Bus>(txt);
            //initiatorSocket[i]->register_nb_transport_bw          (this, &Bus::nb_transport_bw, i);
            initiatorSocket[i]->register_invalidate_direct_mem_ptr(this, &Bus::invalidate_direct_mem_ptr, i);
        }
    }

    void b_transport(int id, tlm::tlm_generic_payload& trans, sc_time& delay) {
        if (id >= initiatorNum) {
            char errorMsg[100];
            sprintf(errorMsg, "b_transport error, id = %d", id);
            SC_REPORT_ERROR("TLM2", errorMsg);
            return;
        }

        uint64 address = trans.get_address();
        uint64 masked_address;
        unsigned int target_nr = decode_address(address, masked_address);
        //cout << "Target to send = " << target_nr << endl;
        if (target_nr >= targetNum) {
            cout << "target_nr exceeds max target number." << endl;
            return;
        }
        trans.set_address(masked_address);
        (*initiatorSocket[target_nr])->b_transport(trans, delay);
        trans.set_address(address);
    }

    bool get_direct_mem_ptr(int id, tlm::tlm_generic_payload& trans, tlm::tlm_dmi& dmiData) {
        uint64 masked_address;
        unsigned int target_nr = decode_address(trans.get_address(), masked_address );
        //printf("Target to send = %u ; address = %u\n", target_nr, static_cast<unsigned int>(trans.get_address()));
        if (target_nr >= targetNum) {
            cout << "target_nr exceeds max target number." << endl;
            return false;
        }
        trans.set_address(masked_address);
        bool status = (*initiatorSocket[target_nr])->get_direct_mem_ptr(trans, dmiData);
        dmiData.set_start_address(compose_address(target_nr, dmiData.get_start_address()));
        dmiData.set_end_address(compose_address(target_nr, dmiData.get_end_address()));
        return status;
    }

    void invalidate_direct_mem_ptr(int id, sc_dt::uint64 start_range, sc_dt::uint64 end_range) {
        uint64 bw_start_range = compose_address(id, start_range );
        uint64 bw_end_range = compose_address(id, end_range );
        for(int i=0 ; i<initiatorNum ; i++) {
            (*targetSocket[i])->invalidate_direct_mem_ptr(bw_start_range, bw_end_range);
        }
    }

    inline uint64 compose_address(unsigned int target_nr, uint64 address) {
        return (target_nr << 6) | (address & 0x3F);
    }

    inline unsigned int decode_address(uint64 address, uint64& masked_address ) {
        unsigned int target_nr = static_cast<unsigned int>((address >> 6) & 0x3);
        masked_address = address & 0x3F;
        return target_nr;
    }
};

#endif