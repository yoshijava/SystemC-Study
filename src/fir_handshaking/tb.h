#include <systemc>

using namespace sc_core;
using namespace sc_dt;

SC_MODULE(tb) {
    sc_in<bool> clk;
    sc_out<bool> reset;

    sc_out< sc_int<16> > in;
    sc_out<bool> in_valid;
    sc_in<bool> in_ready;

    sc_in< sc_int<16> > out;
    sc_in<bool> out_valid;
    sc_out<bool> out_ready;



    void source();
    void sink();

    SC_CTOR(tb) {
        SC_CTHREAD(source, clk.pos());
        SC_CTHREAD(sink, clk.pos());
    }
};
