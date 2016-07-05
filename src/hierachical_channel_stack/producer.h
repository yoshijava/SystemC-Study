#ifndef PRODUCER_H
#define PRODUCER_H

#include "systemc.h"
#include "stackInterface.h"

class producer : public sc_module {
public:

    sc_port<stack_write_if> out;
    sc_in<bool> Clock;

    void do_writes();

    SC_CTOR(producer) {
        SC_THREAD(do_writes);
        sensitive << Clock.pos();
    }
};

#endif
