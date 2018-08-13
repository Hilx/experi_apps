#include <stdio.h>
#include <stdlib.h>
#include "top.h"

int main(void){
	ptr_t ptr_root = root_create();

	for(int i = 0; i < 200; i++){
		int a = RandGen(7);
		int b = RandGen(7);
		graph_addedge(ptr_root, a, b);
	}

	graph_printout(ptr_root);
}