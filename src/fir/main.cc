#include <systemc>
#include "fir.h"
#include "tb.h"

using namespace std;
using namespace sc_core;
using namespace sc_dt;

SC_MODULE(SYSTEM) {
    tb *tb0;
    fir *fir0;

    sc_signal< sc_int<16> > input_signal;
    sc_signal< sc_int<16> > output_signal;
    sc_signal<bool> reset_signal;
    sc_clock clock_signal;

    SC_CTOR(SYSTEM) : clock_signal("clock_signal", 10, SC_NS) {
        // TODO: Why the following line is not equivalent?
        // clock_signal("clock_signal", 10, SC_NS);

        tb0 = new tb("tb0");
        tb0->clk(clock_signal);
        tb0->reset(reset_signal);
        tb0->input(input_signal);
        tb0->output(output_signal);

        fir0 = new fir("fir0");
        fir0->clk(clock_signal);
        fir0->reset(reset_signal);
        fir0->input(input_signal);
        fir0->output(output_signal);
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
