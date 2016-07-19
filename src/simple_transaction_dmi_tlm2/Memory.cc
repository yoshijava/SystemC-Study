#include "Memory.h"

using namespace sc_core;
using namespace sc_dt;
using namespace std;
using namespace tlm;

void Memory::memoryProcess() {
    // Simply invalidate dmi pointer periodically
    sc_time latency = sc_time(5, SC_NS);
    for (int i=0 ; i<96 ; i++) {
        wait(latency);
        if (i % 8 == 0) {
            socket->invalidate_direct_mem_ptr(0, MEM_SIZE-1);
        }
    }
}

void Memory::b_transport(tlm_generic_payload& transactions, sc_time& delay) {
    tlm_command     cmd =            transactions.get_command();     
    sc_dt::uint64   address =        transactions.get_address() / 4;
    unsigned char*  dataPtr =        transactions.get_data_ptr();
    unsigned int    dataLength =     transactions.get_data_length();
    unsigned int    streamingWidth = transactions.get_streaming_width();
    // unsigned char* byteEnablePtr = transactions.get_byte_enable_ptr();
    // unsigned int byteEnableLength = transactions.get_byte_enable_length();
    
    if (address < 0 || address >  MEM_SIZE) {
        transactions.set_response_status(TLM_ADDRESS_ERROR_RESPONSE);
        return;
    }
    if (dataLength > 4 || dataLength > streamingWidth) {
        transactions.set_response_status(TLM_BURST_ERROR_RESPONSE);
        return;
    }

    if (cmd == TLM_READ_COMMAND) {
        memcpy(dataPtr, &memory[address], dataLength);
    } else if (cmd == TLM_WRITE_COMMAND) {
        memcpy(&memory[address], dataPtr, dataLength);
    }

    wait(delay);

    transactions.set_dmi_allowed(true);
    transactions.set_response_status(TLM_OK_RESPONSE);
}

bool Memory::get_direct_mem_ptr(tlm_generic_payload& transactions, tlm_dmi& dmiData) {
    dmiData.set_dmi_ptr(reinterpret_cast<unsigned char*>(memory));
    dmiData.set_start_address(0);
    dmiData.set_end_address(MEM_SIZE*4 - 1);
    dmiData.set_read_latency(sc_time(5, SC_NS));
    dmiData.set_write_latency(sc_time(5, SC_NS));
    //dmiData.allow_read();
    //dmiData.allow_write();
    //dmiData.set_granted_access(DMI_ACCESS_READ_WRITE);
    dmiData.allow_read_write();    // The same as set_granted_access(DMI_ACCESS_READ_WRITE)
}