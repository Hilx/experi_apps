#include "malloc_wrapper.h"

ptr_t custom_malloc(data_t size){
	data_t * ptr = (data_t *)malloc(size);
	ptr_t ptr2return = (data_t)ptr;
	return ptr2return;
}

void custom_free(ptr_t ptr){
	data_t * ptr_i;
	ptr_i = ptr;
	free(ptr_i); 
}