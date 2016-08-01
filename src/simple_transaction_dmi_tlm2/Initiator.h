#ifndef INITIATOR_H
#define INITIATOR_H

#include "systemc.h"
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"

SC_MODULE(Initiator) {
    tlm_utils::simple_initiator_socket<Initiator> socket;
    tlm::tlm_dmi dmiData;
    bool isDmiValid;
    bool errorAfter15Ns;

    SC_HAS_PROCESS(Initiator);
    //SC_CTOR(Initiator) : socket("socket"), isDmiValid(false) {
    Initiator(sc_module_name name_, bool errorAfter15Ns_=false) : sc_module(name_), socket("socket"), isDmiValid(false), errorAfter15Ns(errorAfter15Ns_) {

        socket.register_invalidate_direct_mem_ptr(this, &Initiator::invalidate_direct_mem_ptr);
        SC_THREAD(initiatorProcess);
    }

    void initiatorProcess();
    virtual void invalidate_direct_mem_ptr(sc_dt::uint64 start_range, sc_dt::uint64 end_range);
};

#endif