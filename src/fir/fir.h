#include <systemc>

using namespace sc_dt;

namespace fir {
    const sc_uint<8> coef[5] = {3,0,6,7,8};
    sc_in<bool> clk;
    sc_in<bool> reset;
    sc_in< sc_int<16> > input;
    sc_in< sc_int<16> > output;

    void fir_main(void);
}
