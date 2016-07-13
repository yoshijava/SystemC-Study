#include "producer.h"

void producer::do_writes() {
    int i = 0;
    const char* testString = "Hello, this will NOT be reversed!!!!!";
    while (true) {
        wait(); // for clock edge
        out->write(testString[i]);
        cout << "W " << testString[i] << " at " << sc_time_stamp() << endl;
        i = (i+1) % strlen(testString);
    }
}
