#ifndef PRODUCER_H
#define PRODUCER_H

#include "systemc.h"
#include "fifoInterface.h"

class producer : public sc_module {
public:

    sc_port<fifo_interface> out;
    sc_in<bool> clock;

    void do_writes();

    SC_CTOR(producer) {
        SC_THREAD(do_writes);
        sensitive << clock.pos();
    }
};

#endif
