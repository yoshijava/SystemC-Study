#ifndef BUS_H
#define BUS_H

#include "systemc.h"
#include "tlm.h"
#include "tlm_utils/multi_passthrough_initiator_socket.h"
#include "tlm_utils/multi_passthrough_target_socket.h"
using namespace sc_core;
using namespace sc_dt;
using namespace std;
using namespace tlm;
using namespace tlm_utils;

SC_MODULE(Bus) {
    multi_passthrough_target_socket<Bus> targetSocket;
    multi_passthrough_initiator_socket<Bus> initiatorSocket;
    map<tlm_generic_payload*, unsigned int> mMap;

    SC_CTOR(Bus) : targetSocket("targetSocket"), initiatorSocket("initiatorSocket") {
        targetSocket.register_nb_transport_fw   (this, &Bus::nb_transport_fw);
        targetSocket.register_b_transport       (this, &Bus::b_transport);
        targetSocket.register_get_direct_mem_ptr(this, &Bus::get_direct_mem_ptr);
        
        initiatorSocket.register_nb_transport_bw          (this, &Bus::nb_transport_bw);
        initiatorSocket.register_invalidate_direct_mem_ptr(this, &Bus::invalidate_direct_mem_ptr);
    }

    tlm_sync_enum nb_transport_fw(int id, tlm_generic_payload& trans, tlm_phase& phase, sc_time& delay) {
        
        mMap[&trans] = id;
        uint64 address = trans.get_address();
        uint64 maskedAddress;
        unsigned int targetID = decode_address(address, maskedAddress);

        if (targetID < initiatorSocket.size()) {
            trans.set_address(maskedAddress);
            tlm_sync_enum result = initiatorSocket[targetID]->nb_transport_fw(trans, phase, delay);
            trans.set_address(address);
            return result;
        } else {
            fout << "targetID exceeds max target number." << endl;
            return TLM_COMPLETED;
        }
    }

    tlm_sync_enum nb_transport_bw(int id, tlm_generic_payload& trans, tlm_phase& phase, sc_time& delay ) {
        uint64 address = trans.get_address();
        uint64 completeAddress = compose_address(id, address );
        trans.set_address(completeAddress);
        return targetSocket[mMap[&trans]]->nb_transport_bw(trans, phase, delay);
    }

    void b_transport(int id, tlm_generic_payload& trans, sc_time& delay) {
        uint64 address = trans.get_address();
        uint64 maskedAddress;
        unsigned int targetID = decode_address(address, maskedAddress);
        //cout << "Target to send = " << targetID << endl;
        if (targetID >= initiatorSocket.size()) {
            fout << "targetID exceeds max target number." << endl;
            return;
        }
        trans.set_address(maskedAddress);
        initiatorSocket[targetID]->b_transport(trans, delay);
        trans.set_address(address);
    }

    bool get_direct_mem_ptr(int id, tlm_generic_payload& trans, tlm_dmi& dmiData) {
        uint64 maskedAddress;
        unsigned int targetID = decode_address(trans.get_address(), maskedAddress );
        //printf("Target to send = %u ; address = %u\n", target_nr, static_cast<unsigned int>(trans.get_address()));
        if (targetID >= initiatorSocket.size()) {
            fout << "targetID exceeds max target number." << endl;
            return false;
        }
        trans.set_address(maskedAddress);
        bool status = initiatorSocket[targetID]->get_direct_mem_ptr(trans, dmiData);
        dmiData.set_start_address(compose_address(targetID, dmiData.get_start_address()));
        dmiData.set_end_address(compose_address(targetID, dmiData.get_end_address()));
        return status;
    }

    void invalidate_direct_mem_ptr(int id, sc_dt::uint64 start_range, sc_dt::uint64 end_range) {
        uint64 bw_start_range = compose_address(id, start_range );
        uint64 bw_end_range = compose_address(id, end_range );
        for(int i=0 ; i<targetSocket.size() ; i++) {
            targetSocket[i]->invalidate_direct_mem_ptr(bw_start_range, bw_end_range);
        }
    }

    // unsigned int transport_dbg(int id, tlm::tlm_generic_payload& trans) {
    //     uint64 maskedAddress;
    //     uint64 address = trans.get_address();
    //     unsigned int targetID = decode_address(address, maskedAddress);
    //     trans.set_address(maskedAddress);
    //     return (*initiatorSocket[targetID])->transport_dbg(trans);
    // }

    inline uint64 compose_address(unsigned int target_nr, uint64 address) {
        return address;
    }

    inline unsigned int decode_address(uint64 address, uint64& masked_address ) {
        unsigned int target_nr = static_cast<unsigned int>(address & 0x3);
        masked_address = address;
        return target_nr;
    }

};

#endif