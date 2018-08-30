#include "malloc_wrapper.h"
#include "fake_malloc.h"

// ----------------------------------------------- //
// ZEPALLOC
// ----------------------------------------------- //
ptr_t custom_malloc(data_t * m2mem, data_t * m2alloc, data_t size){
	//ptr_t temp = C_OFF;
	//C_OFF = C_OFF + size/4;
	//printf("custom malloc result = %d\n", C_OFF);
	//return temp*4;

	const data_t MALLOC_THRE[MAX_ALLOC_INDEX + 1] = {12, 28, 60, 124, 252, 508, 1020, 2044, 4092, 8188, 16380, 32764, 65532, 131068, 262140,524284,1048572,2097148,4194300,8388604, 16777212};
	data_t i = 0;
	data_t flag = 0;
	ptr_t ptr;
	FIND_REG:while(flag == 0){
		if(size > MALLOC_THRE[i]){
			i++;
		}else{
			flag = 1;
			ptr = m2alloc[(BASE + i * 4 )/4];
		}
	}
	return ptr;
}

volatile void custom_free(data_t * m2mem, data_t * m2alloc, ptr_t ptr){
	m2alloc[BASE/4] = ptr;
}

data_t custom_alloc_init(data_t * m2mem, data_t * m2alloc){
	data_t mbase = 0x10000000;
	data_t mtotal = 0x2000000;
	data_t temp = 0;
	int j;
	for(j = 0; j < mtotal; j++ ){
		m2mem[(mbase + 4*j)/4] = 0;
		if(j == mtotal - 1){
			data_t temp_i = m2alloc[RESE/4];
			m2alloc[INIT/4] = temp_i;
			data_t token = m2alloc[INIT/4];
			while(token == 2){
				token = m2alloc[INIT/4];
			}
			temp = 1;
		}
	}
	return temp;
}

// ----------------------------------------------- //


// ----------------------------------------------- //
// SYSALLOC
// ----------------------------------------------- //
/*
ptr_t custom_malloc(data_t * m2mem, data_t * m2alloc, data_t size){
	int token = 0, status = 0, result = 0;
	while(token == 0){
    	token = m2alloc[(MMU_BASE + MMU_TOKEN)/4];
	}
	m2alloc[(MMU_BASE+ MMU_CMD)/4] = size;
	while(status == 0){
		status = m2alloc[(MMU_BASE+ MMU_STATUS)/4];
	}
	result = m2alloc[(MMU_BASE+ MMU_RESULT)/4];
	return result;
}

volatile void custom_free(data_t * m2mem, data_t * m2alloc, ptr_t ptr){
	int token = 0, status = 0, result = 0;
	while(token == 0){
    	token = m2alloc[(MMU_BASE + MMU_TOKEN)/4];
	}
	m2alloc[(MMU_BASE+ MMU_FREE)/4] = addr;
	while(status == 0){
		status = m2alloc[(MMU_BASE+ MMU_STATUS)/4];
	}
	result = m2alloc[(MMU_BASE+ MMU_RESULT)/4];
}

volatile void custom_alloc_init(data_t * m2mem, data_t * m2alloc){
	int i;
	for(i = 0; i < 2396745; i++)
		m2mem[0x18000000/4 + i] = 0;
	}
}
*/
