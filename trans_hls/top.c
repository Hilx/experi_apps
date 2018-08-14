#include "top.h"

int transcore(data_t * slaveAxi, data_t * m2mem, data_t * m2alloc, data_t timer_addr){
	#pragma HLS INTERFACE s_axilite port=slaveAxi   bundle=SlavePort
	#pragma HLS INTERFACE s_axilite port=return     bundle=SlavePort
	#pragma HLS INTERFACE s_axilite port=timer_addr bundle=SlavePort
	#pragma HLS INTERFACE m_axi	depth=1	port=m2mem	 offset=off
	#pragma HLS INTERFACE m_axi depth=1 port=m2alloc offset=off
	// buffer for burst
	data_t BUFFER_FOR_BURSTING[11];
	data_t * buffer = BUFFER_FOR_BURSTING; 
	// taking in run configs through axi slave interface
	int timer_base = timer_addr;
	
	return 0;
}