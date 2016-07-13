#include <systemc>
#include "fir.h"
#include "tb.h"

using namespace std;
using namespace sc_core;
using namespace sc_dt;

SC_MODULE(SYSTEM) {
    tb *tb0;
    fir *fir0;

    sc_signal< sc_int<16> > signal_input;
    sc_signal<bool> in_signal_valid;
    sc_signal<bool> in_signal_ready;

    sc_signal< sc_int<16> > signal_output;
    sc_signal<bool> out_signal_valid;
    sc_signal<bool> out_signal_ready;

    sc_signal<bool> signal_reset;
    sc_clock clock_signal;

    SC_CTOR(SYSTEM) : clock_signal("clock_signal", 10, SC_NS) {
        // TODO: Why the following line is not equivalent?
        // clock_signal("clock_signal", 10, SC_NS);

        tb0 = new tb("tb0");
        tb0->clk(clock_signal);
        tb0->reset(signal_reset);

        tb0->in(signal_input);
        tb0->in_valid(in_signal_valid);
        tb0->out_ready(out_signal_ready);

        tb0->out(signal_output);
        tb0->out_valid(out_signal_valid);
        tb0->in_ready(in_signal_ready);

        fir0 = new fir("fir0");
        fir0->clk(clock_signal);
        fir0->reset(signal_reset);

        fir0->in(signal_input);
        fir0->in_valid(in_signal_valid);
        fir0->out_ready(out_signal_ready);

        fir0->out(signal_output);
        fir0->out_valid(out_signal_valid);
        fir0->in_ready(in_signal_ready);
    }

    ~SYSTEM() {
        delete tb0;
        delete fir0;
    }
};

SYSTEM *top = NULL;

int sc_main(int count, char* args[]) {
    top = new SYSTEM("top");
    cout << "About to start the simulation..." << endl;
    sc_start();
    return 0;
}
