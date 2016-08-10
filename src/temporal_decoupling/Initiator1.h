#ifndef INITIATOR1_H
#define INITIATOR1_H

#include "systemc.h"
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/tlm_quantumkeeper.h"

using namespace sc_core;
using namespace sc_dt;
using namespace std;
using namespace tlm;
using namespace tlm_utils;

SC_MODULE(Initiator1) {
    simple_initiator_socket<Initiator1> socket;
    tlm_dmi dmiData;
    tlm_quantumkeeper mQuantumkeeper;
    bool isDmiValid;
    int data;

    SC_CTOR(Initiator1) : socket("socket"), isDmiValid(false) {
        socket.register_invalidate_direct_mem_ptr(this, &Initiator1::invalidate_direct_mem_ptr);
        SC_THREAD(initiator1Process);
        mQuantumkeeper.set_global_quantum(sc_time(1000, SC_NS));
        mQuantumkeeper.reset();
    }

    void initiator1Process() {
        dumpMemory();
        sc_time delay;
        tlm_generic_payload* transaction = new tlm_generic_payload();

        for (int i=0 ; i<256 ; i+=4) {
            data = 0xFF000000 | i;
            delay = mQuantumkeeper.get_local_time();

            if (isDmiValid) {
                unsigned char* dmiPtr = dmiData.get_dmi_ptr();
                uint64 startAddress = dmiData.get_start_address();

                memcpy(dmiPtr + i - startAddress, &data, 4);
                //wait(dmiData.get_write_latency());    //No waiting?
                delay += dmiData.get_write_latency();
                
                cout << "DMI WRITE addr = " << hex << i << ", data = " << data
                << " at " << sc_time_stamp() << " delay = " << delay << endl;
            } else {
                // Set all attributes except byte_enable_length and extensions (unused)
                transaction->set_command(TLM_WRITE_COMMAND);
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

                // Get the answer from Memory
                if (transaction->is_dmi_allowed()) {
                    dmiData.init();
                    isDmiValid = socket->get_direct_mem_ptr(*transaction, dmiData);
                }

                cout << "WRITE     addr = " << hex << i << ", data = " << data
                << " at " << sc_time_stamp() << " delay = " << delay << "\n";
            }

            mQuantumkeeper.set(delay);
            mQuantumkeeper.inc(sc_time(100, SC_NS));
            if (mQuantumkeeper.need_sync()) {
                mQuantumkeeper.sync();
            }
        }
        wait(2, SC_US);
        dumpMemory();
    }

    void invalidate_direct_mem_ptr(sc_dt::uint64 start_range, sc_dt::uint64 end_range) {
        cout << "INVALIDATE DMI (" << start_range << ".." << end_range
            << ") for Initiator1 at " << sc_time_stamp() << "\n";
        isDmiValid = false;
    }

    void dumpMemory() {
        cout << "----------------------Dump memory----------------------" << endl;
        unsigned char buffer[MEM_SIZE];
        for (int i=0 ; i<4 ; i++) {
            tlm_generic_payload debug;
            debug.set_address(MEM_SIZE * i);
            debug.set_command(TLM_READ_COMMAND);
            debug.set_data_length(MEM_SIZE);
            debug.set_data_ptr(buffer);

            unsigned int readLength = socket->transport_dbg(debug);

            for (int j=0 ; j<readLength ; j+=4) {
                cout << "mem[" << hex << (MEM_SIZE*i + j) << "] = "
                    << *(reinterpret_cast<unsigned int*>( &buffer[j] )) << endl;
            }
        }
        cout << "--------------------------------------------------------" << endl;
    }
};

#endif