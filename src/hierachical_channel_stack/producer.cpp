#include "producer.h"

void producer::do_writes()
{
    int i = 0;
    const char* TestString = "Hallo,     This Will Be Reversed";
    while (true){
        wait();             // for clock edge
        if (out->nb_write(TestString[i])){
            cout << "W " << TestString[i] << " at " << sc_time_stamp() << endl;
        }
        i = (i+1) % 32;
    }
}