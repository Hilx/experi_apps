#include "top.h"
#include <stdio.h>

int graphcore(data_t * slaveAxi, data_t * m2mem, data_t * m2alloc, data_t timer_addr, data_t num_iter, data_t rng_range){

#pragma HLS INTERFACE s_axilite port=slaveAxi   bundle=SlavePort
#pragma HLS INTERFACE s_axilite port=return     bundle=SlavePort
#pragma HLS INTERFACE s_axilite port=timer_addr bundle=SlavePort
#pragma HLS INTERFACE s_axilite port=num_iter 	bundle=SlavePort
#pragma HLS INTERFACE s_axilite port=rng_range 	bundle=SlavePort
#pragma HLS INTERFACE m_axi	depth=1	port=m2mem	 offset=off
#pragma HLS INTERFACE m_axi depth=1 port=m2alloc offset=off

// buffer for burst
data_t BUFFER_FOR_BURSTING[11];
data_t * buffer = BUFFER_FOR_BURSTING;

// taking in run configs through axi slave interface
int timer_base = timer_addr;
int n = num_iter;
int r = rng_range;

// initialise allocator
data_t flag = 0;
flag = custom_alloc_init(m2mem, m2alloc);

// initialise rng
rng_init();

ptr_t rootPtr;
data_t c = 0;

if(flag == 1){
	// stage 1 - graph creation
	int i;
	for(i = 0; i < n; i++){
		if(i == 0){
			m2alloc[(timer_base)/4 + 1] = C_START;
			// create graph root
			rootPtr = root_create(m2mem, m2alloc, buffer);
		}
		//printf(" ---- Run %d ----\n", i);
		data_t a = RandGen(r);
		data_t b = RandGen(r);
		graph_addedge(m2mem, m2alloc, buffer, rootPtr, a, b);
		if(i == n - 1){
			m2alloc[(timer_base)/4 + 1] = C_STOP;
			// stage 2 - graph access & return keycnt
			m2alloc[(timer_base)/4 + 2] = C_START;
			c = 0;
			c = graph_printout(m2mem, m2alloc, buffer, rootPtr);
			if(c != 0){
				m2alloc[(timer_base)/4 + 2] = C_STOP;
			}
		}
	}
}

return c;
}
