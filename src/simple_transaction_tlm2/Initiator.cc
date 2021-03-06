#include "Initiator.h"
// Initiator module generating generic payload transactions
using namespace tlm;

void Initiator::thread_process() {
    // TLM-2 generic payload transaction, reused across calls to b_transport
    tlm_generic_payload* trans = new tlm_generic_payload();
    sc_time delay = sc_time(10, SC_NS);

    // Generate a random sequence of reads and writes
    for (int i = 32; i < 96; i += 4) {
        tlm_command cmd = static_cast<tlm_command>(rand() % 2);
        if (cmd == TLM_WRITE_COMMAND) {
            data = 0xFF000000 | i;
        }

        // Initialize 8 out of the 10 attributes, byte_enable_length and extensions being unused
        trans->set_command(cmd);
        trans->set_address(i);
        trans->set_data_ptr(reinterpret_cast<unsigned char*>(&data));
        trans->set_data_length(4);
        trans->set_streaming_width(4); // = data_length to indicate no streaming
        trans->set_byte_enable_ptr(0); // 0 indicates unused
        trans->set_dmi_allowed(false); // Mandatory initial value
        trans->set_response_status( TLM_INCOMPLETE_RESPONSE ); // Mandatory initial value

        socket->b_transport(*trans, delay);  // Blocking transport call

        // Initiator obliged to check response status and delay
        if (trans->is_response_error()) {
            SC_REPORT_ERROR("TLM-2", "Response error from b_transport");
        }

        cout << "trans = { " << (cmd ? 'W' : 'R') << ", " << hex << i << " } , data = ";
        cout << hex << data << " at time " << sc_time_stamp() << " delay = " << delay << endl;

        // Realize the delay annotated onto the transport call
        wait(delay);
    }
}
