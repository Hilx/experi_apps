/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* XILINX CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "top.h"

#define COUNTER_BASE 0x43C10000
#define START 1
#define STOP  1
#define RESET 3

void print(char *str);

int main()
{
    init_platform();

    print("Hello World\n\r");

    /*
	ptr_t ptr_root = root_create();

	int i;
	for(i = 0; i < 256; i++){
		int a = RandGen(7);
		int b = RandGen(7);
		graph_addedge(ptr_root, a, b);
	}

	graph_printout(ptr_root);
     */

    int i;
    for(i = 20; i < 21; i++){
        init_platform();
    	// test parameters
    	int test_size = i;
    	int num_iter = 1 << i;
    	putnum(test_size);
    	print(" ");
    	int rng_range = 5;
    	if(i > 10){
    		rng_range = i - 5;
    	}
    	// run test
    	Xil_Out32(COUNTER_BASE + 4 * 1, START);
    	ptr_t ptr_root = root_create();
    	int i;
    	for(i = 0; i < num_iter; i++){
    		int a = RandGen(rng_range);
    		int b = RandGen(rng_range);
    		graph_addedge(ptr_root, a, b);
    	}
    	Xil_Out32(COUNTER_BASE + 4 * 1, STOP);
    	Xil_Out32(COUNTER_BASE + 4 * 2, START);
    	graph_printout(ptr_root);
    	Xil_Out32(COUNTER_BASE + 4 * 2, STOP);

    	// print out measurements
    	int j;
		for(j = 1; j <3; j++){
			int high_value = Xil_In32(COUNTER_BASE + 4 * (j + 8));
			if(high_value != 0){
				putnum(high_value);
			}
			putnum(Xil_In32(COUNTER_BASE + 4 * j));
			print(" ");
			Xil_Out32(COUNTER_BASE + 4 * j, RESET);
		}
		print("\n");
	    cleanup_platform();
    }


	print("Run finished.\n\r");

    cleanup_platform();
    return 0;
}
