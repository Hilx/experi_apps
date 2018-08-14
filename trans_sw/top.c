#include <stdio.h>
#include <stdlib.h>
#include "top.h"

int main(void){

	ptr_t ptr_hashtable = hashtable_create();

	struct process_pack_t ipack, opack;
	ipack.ptr_hashtable = ptr_hashtable;
	ipack.ptr_list = NULL_PTR;
	ipack.key = 0;
	ipack.key_valid = 0;

	// testing

	ipack = string_process(ipack, LABEL_BEGIN);
	ipack = string_process(ipack, 7);
	ipack = string_process(ipack, 10);
	ipack = string_process(ipack, 7);
	ipack = string_process(ipack, 11);
	ipack = string_process(ipack, LABEL_BEGIN);
	ipack = string_process(ipack, 7);
	ipack = string_process(ipack, 5);
	ipack = string_process(ipack, 7);
	ipack = string_process(ipack, 4);
	ipack = string_process(ipack, LABEL_END);

	// printing
	print_table(ptr_hashtable);


	ipack = string_process(ipack, LABEL_BEGIN);
	ipack = string_process(ipack, 32);
	ipack = string_process(ipack, 1);
	ipack = string_process(ipack, 128);
	ipack = string_process(ipack, 2);	
	ipack = string_process(ipack, LABEL_BEGIN);	
	ipack = string_process(ipack, 256);
	ipack = string_process(ipack, 3);
	ipack = string_process(ipack, 64);
	ipack = string_process(ipack, 4);
	ipack = string_process(ipack, 0);
	ipack = string_process(ipack, 5);			
	ipack = string_process(ipack, LABEL_END);

	// printing
	print_table(ptr_hashtable);

	return 0;
}