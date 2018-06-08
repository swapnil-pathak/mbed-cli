#include "mbed.h"
#include "afl_call.h" 

Serial pc(USBTX, USBRX); // tx, rx
 
int noHyperCall = 0; // dont make hypercalls
int verbose = 0; // XXX Never turns on verbose mode, printf/scanf will hang mbed 

int main() {
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
//    pc.printf("Hello World!\n");
//    int i = 0;
    while(1) {
        pc.putc(pc.getc() + 1);
//	pc.putc(i++);
	doneWork(0);
    }
}
