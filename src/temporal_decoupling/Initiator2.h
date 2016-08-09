#ifndef INITIATOR2_H
#define INITIATOR2_H

#include "systemc.h"
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/tlm_quantumkeeper.h"
#include "MM.h"

using namespace sc_core;
using namespace sc_dt;
using namespace std;
using namespace tlm;
using namespace tlm_utils;

SC_MODULE(Initiator2) {
    simple_initiator_socket<Initiator2> socket;
    MM mMM;
    tlm_quantumkeeper mQuantumkeeper;
    int data;

    SC_CTOR(Initiator2) : socket("socket") {
        SC_THREAD(initiator2Process);

        // No need to set global quantum again because this is a static call
        // and has been done in Initiator1
        //mQuantumkeeper.set_global_quantum(sc_time(1000, SC_NS));
        mQuantumkeeper.reset();
    }

    void initiator2Process() {
        sc_time delay;
        tlm_generic_payload* transaction = new tlm_generic_payload();

        wait(1, SC_US);

        for (int i=0 ; i<256 ; i+=4) {
            delay = mQuantumkeeper.get_local_time();
            // Grab a new transactionaction from the memory manager
            transaction = mMM.allocate();
            transaction->acquire();

            // Set all attributes except byte_enable_length and extensions (unused)
            transaction->set_command(TLM_READ_COMMAND);
            transaction->set_address(i);
            transaction->set_data_ptr(reinterpret_cast<unsigned char*>(&data));
            transaction->set_data_length(4);
            transaction->set_streaming_width(4); // = data_length to indicate no streaming
            transaction->set_byte_enable_ptr(0); // 0 indicates unused
            transaction->set_dmi_allowed(false); // Mandatory initial value
            transaction->set_response_status(TLM_INCOMPLETE_RESPONSE); // Mandatory initial value

            // Transport to Memory via Bus
            // Memory may allow direct memory access here
            socket->b_transport(*transaction, delay);

            if (transaction->is_response_error()) {
                char errorMsg[100];
                sprintf(errorMsg, "b_transport error and got response string: %s", transaction->get_response_string().c_str());
                SC_REPORT_ERROR("TLM2", errorMsg);
            }

            cout << "READ     addr = " << hex << i << ", data = " << data
                << " at " << sc_time_stamp() << " delay = " << delay << "\n";

            mQuantumkeeper.set(delay);
            mQuantumkeeper.inc(sc_time(100, SC_NS));
            if (mQuantumkeeper.need_sync()) {
                mQuantumkeeper.sync();
            }
        }
    }
};

#endif