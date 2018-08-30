#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xgraphcore.h"

#define COUNTER_BASE 0x43C10000
#define RESET 3

int main()
{
    init_platform();

    print("Hello World\n\r");

    int i;
    for(i = 15; i < 21; i++){

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
    	XGraphcore ip;
    	XGraphcore_Initialize(&ip, XPAR_GRAPHCORE_0_DEVICE_ID);
    	XGraphcore_Set_timer_addr(&ip, COUNTER_BASE);
    	XGraphcore_Set_num_iter(&ip,  num_iter);
    	XGraphcore_Set_rng_range(&ip, rng_range);
    	XGraphcore_Start(&ip);
    	while(XGraphcore_IsDone(&ip) == 0);

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
    }
    print("all done\n");

    cleanup_platform();
    return 0;
}
