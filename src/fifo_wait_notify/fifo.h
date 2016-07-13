#ifndef FIFO_H
#define FIFO_H

#include "systemc.h"
#include "fifoInterface.h"
class fifo : public sc_prim_channel, public fifo_interface {
public:
    fifo (int size=1024) : sc_prim_channel(sc_gen_unique_name("myfifo")){
        mSize = size;
        buffer = new char[size];
        writeIndex = 0;
        readIndex = 0;
        mDebug = false;
    }
    // Inherited from fifo_interface
    char read();
    void write(char charToWrite);
    bool full();
    int elementNum();
    // Inherited from sc_prim_channel
    void update();

private:
    int mElementNum;
    int mSize;
    char *buffer;
    int writeIndex;
    int readIndex;
    sc_event data_read_event;
    sc_event data_written_event;

    bool mDebug;
};

#endif
