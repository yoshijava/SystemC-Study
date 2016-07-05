#include "stack.h"

bool stack::nb_write(char c) {
    if (top < MAX_STACK_SIZE) {
        data[top++] = c;
        return true;
    }
    return false;
}

void stack::reset() {
    top = 0;
}

bool stack::nb_read(char& c) {
    if (top > 0) {
        c = data[--top];
        return true;
    }
    return false;
}

void stack::register_port(sc_port_base& port_, const char* if_typename_) {
    cout << "Binding " << port_.name() << " to interface: " << if_typename_ << endl;
}
