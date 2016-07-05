#include "consumer.h"

void consumer::do_reads()
{
    int i = 0;
    char receivedString[MAX_RECEIVED_SIZE];
    for (int j=0 ; j<MAX_RECEIVED_SIZE ; j++) {
        receivedString[j] = 0;
    }

    while (true)
    {
        wait();             // for clock edge
        if (in->nb_read(receivedString[i])) {
            cout << "R " << receivedString[i] << " at " << sc_time_stamp() << endl;
        }
        i = (i+1) % MAX_RECEIVED_SIZE;
    }
}
