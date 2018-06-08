#include "mbed.h"
#include "afl_call.h"


// hardcoded options 1(global)
int noHyperCall = 0; // dont make hypercalls
int verbose = 0; // XXX Never turns on verbose mode, printf/scanf will hang mbed

const double pi = 3.141592653589793238462;
const double amplitude = 0.5f;
const double offset = 65535/2;

// The sinewave is created on this pin
AnalogOut aout(A5);

int main()
{

    char *buf;
    uint32_t sz;

    // hardcode options 2(local)
    int enableTimer = 0; // enable qemu's timer in forked children

    // step 1: start forkserver
    startForkserver(enableTimer);

    // step 2: get input & enable pm
    buf = getWork(&sz);
    if (sz == -1) {
        // hack to set aflStart to 1 which is required by doneWork
        startWork(0x0U, 0x0U);
        doneWork(0);
    }

    // step 3: start tracing
    startWork(0x0U, 0xffffffffU);

    if (buf[0] == 0xfb && buf[1] == 0x17) {
        // To launch afl w/o hang
        doneWork(0);
}
    double rads = 0.0;
    uint16_t sample = 0;
    
    while(1) {
        // sinewave output
        for (int i = 0; i < 360; i++) {
            rads = (pi * i) / 180.0f;
            sample = (uint16_t)(amplitude * (offset * (cos(rads + pi))) + offset);
            aout.write_u16(sample);
        }
	doneWork(0);
    }
}
 

