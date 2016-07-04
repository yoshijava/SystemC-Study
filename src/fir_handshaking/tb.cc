#include "tb.h"
using namespace std;
using namespace sc_core;
using namespace sc_dt;

void tb::source() {
    sc_int<16> tmp;

    // Reset
    in_valid.write(0);
    reset.write(1);
    wait();
    reset.write(0);
    wait();

    // Send stimulus to FIR
    for(int i=0; i<64; i++) {
        if(i>23 & i<29) {
            tmp = 256;
        }
        else {
            tmp = 0;
        }
        cout << "tb writes: " << tmp << endl;
        in.write(tmp);
        in_valid.write(1);
        do {
            wait();
        } while(!in_ready.read());
        in_valid.write(0);
        wait();
    }
}

void tb::sink() {
    sc_int<16> indata;

    // Read output coming from DUT
    for(int i=0; i<64; i++) {
        indata = out.read();
        cout << i << " :\t" << indata.to_int() << endl;
        out_ready.write(1);
        wait();
    }
    sc_stop();
}
