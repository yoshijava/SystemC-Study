#include <systemc>
#include "fir.hpp"

using namespace sc_core;
using namespace sc_dt;

SC_MODULE(fir) {
    void fir_main();

    SC_CTOR(fir) {
        SC_CTHREAD(fir_main, clk.pos());
        reset_signal_is(reset, true);
    }
};

int sc_main(int count, char* args[]) {
    // TODO
    return 0;
}
