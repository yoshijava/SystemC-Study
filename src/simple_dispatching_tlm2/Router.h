#ifndef ROUTER_H
#define ROUTER_H

#include "systemc.h"
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"

struct Router : sc_module {
    int N_TARGETS;
    tlm_utils::simple_target_socket<Router> target_socket;
    tlm_utils::simple_initiator_socket_tagged<Router>** initiator_socket;

    Router(int N_TARGETS): sc_module("Router"), target_socket("target_socket") {
        target_socket.register_b_transport(this, &Router::b_transport);
        target_socket.register_get_direct_mem_ptr(this, &Router::get_direct_mem_ptr);
        target_socket.register_transport_dbg(this, &Router::transport_dbg);

        using namespace tlm_utils;
        initiator_socket = (simple_initiator_socket_tagged<Router>**) malloc(sizeof(simple_initiator_socket_tagged<Router>*)*N_TARGETS);
        for (int i = 0; i < N_TARGETS; i++) {
            char txt[20];
            sprintf(txt, "socket_%d", i);
            initiator_socket[i] = new tlm_utils::simple_initiator_socket_tagged<Router>(txt);
            initiator_socket[i]->register_invalidate_direct_mem_ptr(this, &Router::invalidate_direct_mem_ptr, i);
        }
    }

    void invalidate_direct_mem_ptr(int id, sc_dt::uint64 start_range, sc_dt::uint64 end_range) {
        uint64 bw_start_range = compose_address(id, start_range );
        uint64 bw_end_range = compose_address(id, end_range );
        target_socket->invalidate_direct_mem_ptr(bw_start_range, bw_end_range);
    }

    void b_transport(tlm::tlm_generic_payload& trans, sc_time& delay) {
        uint64 address = trans.get_address();
        uint64 masked_address;
        unsigned int target_nr = decode_address(address, masked_address);
        trans.set_address(masked_address);
        (*initiator_socket[target_nr])->b_transport(trans, delay );
        std::cout << "Target to send = " << target_nr << std::endl;
    }

    bool get_direct_mem_ptr(tlm::tlm_generic_payload& trans, tlm::tlm_dmi& dmiData) {
        uint64 masked_address;
        unsigned int target_nr = decode_address(trans.get_address(), masked_address );
        trans.set_address(masked_address);
        bool status = (*initiator_socket[target_nr])->get_direct_mem_ptr(trans, dmiData);
        dmiData.set_start_address(compose_address(target_nr, dmiData.get_start_address()));
        dmiData.set_end_address(compose_address(target_nr, dmiData.get_end_address()));
        return status;
    }

    unsigned int transport_dbg(tlm::tlm_generic_payload& trans) {
        uint64 masked_address;
        unsigned int target_nr = decode_address(trans.get_address(), masked_address);
        trans.set_address(masked_address);
        return (*initiator_socket[target_nr])->transport_dbg(trans);
    }

    inline uint64 compose_address(unsigned int target_nr, uint64 address) {
        return (target_nr << 8) | (address & 0xFF);
    }

    inline unsigned int decode_address(uint64 address, uint64& masked_address ) {
        unsigned int target_nr = static_cast<unsigned int>((address >> 8) & 0x3);
        masked_address = address & 0xFF;
        return target_nr;
    }

};

#endif
