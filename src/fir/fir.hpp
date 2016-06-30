#include <systemc>

using namespace sc_dt;
using namespace sc_core;

const sc_uint<8> coef[5] = {3,0,6,7,8};
sc_in<bool> clk;
sc_in<bool> reset;
sc_in< sc_int<16> > input;
sc_out< sc_int<16> > output;

void fir_main(void) {
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
        output.write(val);
        sc_core::wait();
    }
}
