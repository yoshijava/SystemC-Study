#ifndef Util_H
#define Util_H
#include <cmath>

#define DEBUG

// Generate a random delay (with power-law distribution) to aid testing and stress the protocol
int rand_ps() {
    int n = rand() % 100;
    n = n * n * n;
    return n / 100;
}
#endif
