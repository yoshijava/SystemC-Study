#include <systemc>
#ifndef FIR_H
#define FIR_H

using namespace sc_dt;
using namespace sc_core;

SC_MODULE(fir) {

    const sc_uint<8> coef[5] = {3,0,6,7,8};
    sc_in<bool> clk;
    sc_in<bool> reset;
    sc_in< sc_int<16> > input;
    sc_out< sc_int<16> > output;

    void fir_main();

    SC_CTOR(fir) {
        SC_CTHREAD(fir_main, clk.pos());
        reset_signal_is(reset, true);
    }
};

#endif
