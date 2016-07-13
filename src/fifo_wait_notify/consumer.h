#ifndef CONSUMER_H
#define CONSUMER_H

#include "systemc.h"
#include "fifoInterface.h"
#define MAX_RECEIVED_SIZE 1024

class consumer : public sc_module {
public:
    sc_port<fifo_interface> in;
    sc_in<bool> Clock;

    void do_reads();

    SC_CTOR(consumer) {
        SC_THREAD(do_reads);
        sensitive << Clock.pos();
    }
};

#endif
