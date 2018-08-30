#include <stdio.h>
#include "top.h"
#include "custom_types.h"


int main(int argc, char *argv[]){

	printf("Running DUT...");
	// interface arguments
	data_t * slaveAxi;
	data_t * m2alloc;
	data_t timer_addr;
	data_t num_iter;
	data_t rng_range;

	printf("start simulation...\n");

	data_t memory_array[30000];
	data_t * m2mem = memory_array;
	timer_addr = 0;
	num_iter = 1024;
	rng_range = 8;

	printf("hey!\n");

	int temp = graphcore(slaveAxi, m2mem, m2alloc, timer_addr, num_iter, rng_range);

	printf("\nSimulation Finished. \n");

	return 0;
}
