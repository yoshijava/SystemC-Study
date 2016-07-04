#include <systemc>
#ifndef FIR_H
#define FIR_H

using namespace sc_dt;
using namespace sc_core;

SC_MODULE(fir) {
    sc_int<16> taps[5];
    const sc_uint<8> coef[5] = {1,5,10,5,1};

    sc_in<bool> clk;
    sc_in<bool> reset;

    sc_in< sc_int<16> > in;
    sc_in<bool> in_valid;
    sc_out<bool> in_ready;

    sc_out< sc_int<16> > out;
    sc_out<bool> out_valid;
    sc_in<bool> out_ready;

    void fir_core();

    SC_CTOR(fir) {
        SC_CTHREAD(fir_core, clk.pos());
        reset_signal_is(reset, true);
    }
};

#endif
