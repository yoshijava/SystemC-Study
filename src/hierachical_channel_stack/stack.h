#include "systemc.h"
#include "stackInterface.h"
#define MAX_STACK_SIZE 1024
// this class implements the virtual functions
// in the interfaces
class stack: public sc_module, public stack_write_if, public stack_read_if {
private:
    char data[MAX_STACK_SIZE];
    int top; // pointer to top of stack

public:
    // constructor
    stack(sc_module_name nm) : sc_module(nm), top(0) {
    }

    bool nb_write(char c);

    void reset();

    bool nb_read(char& c);

    void register_port(sc_port_base& port_, const char* if_typename_);

};
