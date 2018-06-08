/* mbed Example Program
 * Copyright (c) 2006-2014 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include "mbed.h"
#include "afl_call.h"

// Initialize a pins to perform analog input and digital output fucntions
AnalogIn   ain(A0);
DigitalOut dout(LED1);


// hardcoded options 1(global)
int noHyperCall = 0; // dont make hypercalls
int verbose = 0; // XXX Never turns on verbose mode, printf/scanf will hang mbed

int main(void)
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

    while (1) {
        // test the voltage on the initialized analog pin
        //  and if greater than 0.3 * VCC set the digital pin
        //  to a logic 1 otherwise a logic 0
        if(ain > 0.3f) {
            dout = 1;
        } else {
            dout = 0;
        }
	ain.read();
//	ain.read_u16();        
        // print the percentage and 16 bit normalized values
//        printf("percentage: %3.3f%%\n", ain.read()*100.0f);
//        printf("normalized: 0x%04X \n", ain.read_u16());
//        wait(0.2f);
	doneWork(0);
    }
}

