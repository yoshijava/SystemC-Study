#include "Initiator.h"

using namespace sc_core;
using namespace sc_dt;
using namespace std;
using namespace tlm;

void Initiator::initiatorProcess() {
    tlm_generic_payload* transactions = new tlm_generic_payload();
    sc_time transportDelay = sc_time(10, SC_NS);

    for (int i=256-64 ; i<256+64 ; i+=4) {
        tlm_command cmd = static_cast<tlm_command> (rand() % 2);
        int data = 0xFF000000 | i;

        if (isDmiValid) {
            unsigned char* dmiPtr = dmiData.get_dmi_ptr();
            if (cmd == TLM_READ_COMMAND) {
                memcpy(&data, dmiPtr + i - dmiData.get_start_address(), 4);
                wait(dmiData.get_read_latency());

            }
            else if (cmd == TLM_WRITE_COMMAND) {
                memcpy(dmiPtr + i - dmiData.get_start_address(), &data, 4);
                wait(dmiData.get_write_latency());

            }
            cout << "DMI   = { " << (cmd ? 'W' : 'R') << ", " << hex << i
             << " } ,\tdata = " << hex << data << " at time " << sc_time_stamp()
             << " delay = " << (cmd ? dmiData.get_write_latency() : dmiData.get_read_latency()) << endl;

        } else {
            transactions->set_command(cmd);
            transactions->set_address(i);
            transactions->set_data_ptr(reinterpret_cast<unsigned char*>(&data));
            transactions->set_data_length(4);
            transactions->set_streaming_width(4); // = data_length to indicate no streaming
            transactions->set_byte_enable_ptr(0); // unused
            transactions->set_dmi_allowed(false);
            transactions->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE); // Mandatory initial value

            // Transport to Memory
            // Memory may allow direct memory access here
            socket->b_transport(*transactions, transportDelay);

            //if( !transactions->is_response_ok() )
            if (transactions->is_response_error()) {
                // cout << "b_transport error and got response string: " << endl;
                // cout << transactions->get_response_string << endl;
                char errorMsg[100];
                sprintf(errorMsg, "b_transport error and got response string: %s", transactions->get_response_string().c_str());
                SC_REPORT_ERROR("TLM2", errorMsg);
            }

            // Get the answer from Memory
            if (transactions->is_dmi_allowed()) {
                transactions->set_address(i);
                dmiData.init();
            	isDmiValid = socket->get_direct_mem_ptr(*transactions, dmiData);
            }

            cout << "trans = { " << (cmd ? 'W' : 'R') << ", " << hex << i
                << " } ,\tdata = " << hex << data << " at time " << sc_time_stamp()
                << " delay = " << transportDelay << endl;
        }
    }
}

void Initiator::invalidate_direct_mem_ptr(sc_dt::uint64 start_range, sc_dt::uint64 end_range) {
    isDmiValid = false;
}
