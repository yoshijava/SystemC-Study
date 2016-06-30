#include "tb.h"
using namespace std;
using namespace sc_core;
using namespace sc_dt;

void tb::source() {
    sc_int<16> tmp;

    // Send stimulus to FIR
    for(int i=0; i<64; i++) {
        if(i>23 & i<29) {
            tmp = 256;
        }
        else {
            tmp = 0;
        }
        cout << "tb input.write " << tmp << endl;
        input.write(tmp);
        wait();
    }
}

void tb::sink() {
    sc_int<16> indata;

    // Read output coming from DUT
    for(int i=0; i<64; i++) {
        indata = output.read();
        wait();
        cout << i << " :\t" << indata.to_int() << endl;
    }
    sc_stop();
}
