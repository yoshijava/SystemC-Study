#include <systemc>
#include "fir.h"

using namespace std;

void fir::fir_core(void) {
    const sc_uint<8> coef[5] = {3,0,6,7,8};
    sc_int<16> taps[5];
    // reset
    output.write(0);

    sc_core::wait();
    while(true) {
        for(int i=4; i>0; i--) {
            taps[i] = taps[i-1];
        }
        taps[0] = input.read();

        sc_int<16> val;
        for(int i=0; i<5; i++) {
            val += coef[i] * taps[i];
        }
        cout << "fir output = " << val << endl;
        output.write(val);
        sc_core::wait();
    }
}
