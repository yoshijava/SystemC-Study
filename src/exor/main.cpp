#include "systemc.h"
#include "stim.h"
#include "exor2.h"
#include "mon.h"

int sc_main(int argc, char* argv[])
{
  sc_signal<bool> ASig, BSig, FSig;
  sc_clock TestClk("TestClock", 10, SC_NS, 0.5, 1, SC_NS, true);

  stim Stim1("Stimulus");
  Stim1.A(ASig);
  Stim1.B(BSig);
  Stim1.Clk(TestClk);

  exor2 DUT("exor2");
  DUT.A(ASig);
  DUT.B(BSig);
  DUT.F(FSig);

  mon Monitor1("Monitor");
  Monitor1.A(ASig);
  Monitor1.B(BSig);
  Monitor1.F(FSig);
  Monitor1.clk(TestClk);

  sc_trace_file* traceFile;
  traceFile = sc_create_vcd_trace_file("traces_achu");
  traceFile->set_time_unit(1, SC_NS);
  //((vcd_trace_file*)traceFile)->sc_set_vcd_time_unit(-9);

  sc_trace(traceFile, ASig  , "A" );
  sc_trace(traceFile, BSig  , "B" );
  sc_trace(traceFile, FSig  , "F" );
  // sc_trace(traceFile, DUT.S1, "S1");
  // sc_trace(traceFile, DUT.S2, "S2");
  // sc_trace(traceFile, DUT.S3, "S3");
  sc_trace(traceFile, TestClk,"Clk");

  sc_start();  // run forever
  sc_close_vcd_trace_file(traceFile);

  return 0;

}
