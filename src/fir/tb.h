#include <systemc>

using namespace sc_core;
using namespace sc_dt;

SC_MODULE(tb) {
    sc_in<bool> clk;
    sc_out<bool> reset;
    sc_out< sc_int<16> > input;
    sc_in< sc_int<16> > output;

    void source();
    void sink();

    SC_CTOR(tb) {
        SC_CTHREAD(source, clk.pos());
        SC_CTHREAD(sink, clk.pos());
    }
};
