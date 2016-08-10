#ifndef MEMORY_H
#define MEMORY_H

#include "systemc.h"
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"
#include <ctime>

using namespace sc_core;
using namespace sc_dt;
using namespace tlm;

SC_MODULE(Memory) {
    tlm_utils::simple_target_socket<Memory> socket;
    int memory[MEM_SIZE];
    sc_time latency;

    SC_CTOR(Memory) : socket("socket") {
        socket.register_b_transport(this, &Memory::b_transport);
        socket.register_get_direct_mem_ptr(this, &Memory::get_direct_mem_ptr);
        socket.register_transport_dbg(this, &Memory::transport_dbg);

        for (int i=0 ; i<MEM_SIZE ; i++) {
            memory[i] = 0xAA000000 | i;
        }

        latency = sc_time(10, SC_NS);
        SC_THREAD(memoryProcess);
    }

    virtual void b_transport(tlm::tlm_generic_payload& transaction, sc_time& delay) {
        tlm_command     cmd =            transaction.get_command();     
        sc_dt::uint64   address =        transaction.get_address() / 4;
        unsigned char*  dataPtr =        transaction.get_data_ptr();
        unsigned int    dataLength =     transaction.get_data_length();
        unsigned int    streamingWidth = transaction.get_streaming_width();
        // unsigned char* byteEnablePtr = transaction.get_byte_enable_ptr();
        // unsigned int byteEnableLength = transaction.get_byte_enable_length();
        
        if (address < 0 || address >  MEM_SIZE) {
            transaction.set_response_status(TLM_ADDRESS_ERROR_RESPONSE);
            return;
        }
        if (dataLength > 4 || dataLength > streamingWidth) {
            transaction.set_response_status(TLM_BURST_ERROR_RESPONSE);
            return;
        }

        if (cmd == TLM_READ_COMMAND) {
            memcpy(dataPtr, &memory[address], dataLength);
        } else if (cmd == TLM_WRITE_COMMAND) {
            memcpy(&memory[address], dataPtr, dataLength);
        }

        //wait(delay);
        delay += latency;

        transaction.set_dmi_allowed(true);
        transaction.set_response_status(TLM_OK_RESPONSE);
    }
    virtual bool get_direct_mem_ptr(tlm::tlm_generic_payload& transaction, tlm::tlm_dmi& dmiData) {
        dmiData.set_dmi_ptr(reinterpret_cast<unsigned char*>(memory));
        dmiData.set_start_address(0);
        dmiData.set_end_address(MEM_SIZE*4 - 1);
        dmiData.set_read_latency(sc_time(5, SC_NS));
        dmiData.set_write_latency(sc_time(5, SC_NS));
        dmiData.allow_read_write();    // The same as set_granted_access(DMI_ACCESS_READ_WRITE)
    }

    virtual unsigned int transport_dbg(tlm::tlm_generic_payload& trans) {
        tlm_command cmd = trans.get_command();
        unsigned int dataLength = trans.get_data_length();
        uint64 address = trans.get_address() / 4;

        unsigned actualLength = dataLength;
        if (dataLength >= (MEM_SIZE - address) * 4) {
            actualLength = (MEM_SIZE - address) * 4;
        }

        if (cmd == TLM_READ_COMMAND) {
            memcpy(trans.get_data_ptr(), &memory[address], actualLength);
        } else if (cmd == TLM_WRITE_COMMAND) {
            memcpy(&memory[address], trans.get_data_ptr(), actualLength);
        }
        return actualLength;
    }

    void memoryProcess() {
        // Simply invalidate dmi pointer periodically
        sc_time latency = sc_time(3000, SC_NS);
        for (int i=0 ; i<3 ; i++) {
            wait(latency);
            socket->invalidate_direct_mem_ptr(0, MEM_SIZE-1);
        }
    }
};

#endif