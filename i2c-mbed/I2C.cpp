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

// hardcoded options 1(global)
int noHyperCall = 0; // dont make hypercalls
int verbose = 0; // XXX Never turns on verbose mode, printf/scanf will hang mbed

// Read temperature from LM75BD
 
I2C i2c(I2C_SDA , I2C_SCL ); 
 
const int addr7bit = 0x48;      // 7 bit I2C address
const int addr8bit = 0x48 << 1; // 8bit I2C address, 0x90
 
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

    char cmd[2];
    while (1) {
        cmd[0] = 0x01;
        cmd[1] = 0x00;
        i2c.write(addr8bit, cmd, 2);
 
 //       wait(0.5);
 
        cmd[0] = 0x00;
        i2c.write(addr8bit, cmd, 1);
        i2c.read( addr8bit, cmd, 2);
 
 //       float tmp = (float((cmd[0]<<8)|cmd[1]) / 256.0);
 //       printf("Temp = %.2f\n", tmp);
	doneWork(0);
    }
}
