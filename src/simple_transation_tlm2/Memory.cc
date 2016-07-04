#include "systemc"
#include "Memory.h"
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"

using namespace sc_core;
using namespace sc_dt;
using namespace std;
using namespace tlm;

void Memory::b_transport(tlm_generic_payload& trans, sc_time& delay ) {
    tlm_command cmd = trans.get_command();
    uint64 adr = trans.get_address() / 4;
    unsigned char* ptr = trans.get_data_ptr();
    unsigned int len = trans.get_data_length();
    unsigned char* byte = trans.get_byte_enable_ptr();
    unsigned int width = trans.get_streaming_width();

    // Obliged to check address range and check for unsupported features,
    //   i.e. byte enables, streaming, and bursts
    // Can ignore DMI hint and extensions
    // Using the SystemC report handler is an acceptable way of signalling an error

    if (adr >= uint64(SIZE) || byte != 0 || len > 4 || width < len) {
        SC_REPORT_ERROR("TLM-2", "Target does not support given generic payload transaction");
    }

    // Obliged to implement read and write commands
    if (cmd == TLM_READ_COMMAND) {
        memcpy(ptr, &mem[adr], len);
    }
    else if (cmd == TLM_WRITE_COMMAND) {
        memcpy(&mem[adr], ptr, len);
    }

    // Obliged to set response status to indicate successful completion
    trans.set_response_status(TLM_OK_RESPONSE);
}
