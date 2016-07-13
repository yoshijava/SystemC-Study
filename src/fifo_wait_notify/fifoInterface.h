#ifndef FIFO_INTERFACE_H
#define FIFO_INTERFACE_H

class fifo_interface : virtual public sc_interface {
public:
    virtual char read() = 0;
    virtual void write(char) = 0;
    virtual bool full() = 0;
    virtual int elementNum() = 0;
    fifo_interface(){}
private:
};

#endif