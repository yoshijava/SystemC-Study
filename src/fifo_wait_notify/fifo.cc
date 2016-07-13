#include "fifo.h"

char fifo::read() {
    if (mDebug)
        cout << "read(): mElementNum=" << mElementNum << endl;
    if (mElementNum <= 0) {
        wait(data_written_event);
    }
    if (mDebug)
        cout << "read(): After wait: mElementNum=" << mElementNum << endl;
    char cResult = buffer[readIndex];
    readIndex = (readIndex + 1) % mSize;
    mElementNum--;
    request_update();

    return cResult;
}

void fifo::write(char charToWrite) {
    if (mDebug)
        cout << "write(): mElementNum=" << mElementNum << endl;
    if (full()) {
        wait(data_read_event);
    }
    if (mDebug)
        cout << "write(): After wait: mElementNum=" << mElementNum << endl;
    buffer[writeIndex] = charToWrite;
    writeIndex = (writeIndex + 1) % mSize;
    mElementNum++;
    request_update();
}

bool fifo::full() {
    return mElementNum >= mSize ? true : false;
}

int fifo::elementNum() {
    return mElementNum;
}

void fifo::update() {
    if (mDebug)
        cout << "update()" << endl;
    if (mElementNum > 0){
        data_written_event.notify(SC_ZERO_TIME);
    }
    if (!full()) {
        data_read_event.notify(SC_ZERO_TIME);
    }
}