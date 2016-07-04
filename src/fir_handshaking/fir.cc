#include <systemc>
#include "fir.h"

using namespace std;
using namespace sc_core;

void fir::fir_core(void) {
    // initialize handshake
    in_ready.write(0);
    out_valid.write(0);
    sc_core::wait();

    // FIR filter
    while(true) {
        sc_int<16> in_val;
        sc_int<16> out_val;

        do {
            wait();
            cout << "in_valid.read() = " << in_valid.read() << endl;
        } while(!in_valid.read());
        in_val = in.read();
        in_ready.write(1);

        for(int i=4; i>0; i--) {
            taps[i] = taps[i-1];
        }
        taps[0] = in_val;

        for(int i=0; i<5; i++) {
            out_val += coef[i] * taps[i];
        }
        out.write(out_val);
        out_valid.write(1);
        do {
            cout << "out_val = " << out_val << endl;
            wait();
        } while(!out_ready.read());
        out_valid.write(0);

    }
}
