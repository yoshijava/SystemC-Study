#include "systemc.h"
#include "producer.h"
#include "consumer.h"
#include "fifo.h"

int sc_main(int argc, char* argv[]) {
    sc_clock clkFast("Clock_Fast", 50, SC_NS);
    sc_clock clkSlow("Clock_Slow", 100, SC_NS);

    fifo fifo1(16);

    producer p1("P1");
    p1.out(fifo1);
    p1.Clock(clkSlow);

    consumer c1("C1");
    c1.in(fifo1);
    c1.Clock(clkFast);

    sc_start(4000, SC_NS);

    return 0;
}
