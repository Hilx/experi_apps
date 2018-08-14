#include "top.h"

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
	custom_alloc_init(m2mem, m2alloc);

	// initialise rng
	rng_init();

	m2alloc[(timer_base)/4 + 1] = C_START;
	// create graph root
	ptr_t rootPtr = root_create(m2mem, m2alloc, buffer);
	// stage 1 - graph creation
	for(int i = 0; i < n; i++){
		data_t a = RandGen(r);
		data_t b = RandGen(r);
		graph_addedge(m2mem, m2alloc, buffer, rootPtr, a, b);
	}
	m2alloc[(timer_base)/4 + 1] = C_STOP;


	m2alloc[(timer_base)/4 + 2] = C_START;
	// stage 2 - graph access & return keycnt
	data_t c = graph_printout(m2mem, m2alloc, buffer, rootPtr);
	m2alloc[(timer_base)/4 + 2] = C_START;

	return c;
}