#include "trans.h"

struct process_pack_t string_process(data_t * m2mem, data_t * m2alloc, data_t * buffer, struct process_pack_t ipack, data_t data){
	// output
	struct process_pack_t opack;
	opack.ptr_hashtable = ipack.ptr_hashtable;
	opack.ptr_list = ipack.ptr_list;
	opack.key_valid = 0;
	// take out inputs
	ptr_t ptr_hashtable = ipack.ptr_hashtable;
	ptr_t ptr_list = ipack.ptr_list;

	// label checking
	if(data == LABEL_BEGIN){
		if(ipack.ptr_list != NULL_PTR){
			// abort prev stored list because prev trans incomplete
			list_destroy(m2mem, m2alloc, buffer, ipack.ptr_list);
			opack.ptr_list = NULL_PTR;
		}
	}else if(data == LABEL_END){
		if(ipack.ptr_list != NULL_PTR){
			// take nodes out of list and put them into hash table
			ptr_t ptr_hdList = ipack.ptr_list;
			struct list_node_removed_t meta_node = list_node_remove(m2mem, m2alloc, buffer, ptr_hdList);
			hashtable_entry_add(m2mem, m2alloc, buffer, ipack.ptr_hashtable, meta_node.pair);
			while(meta_node.ptr_hdList != NULL_PTR){
				meta_node = list_node_remove(m2mem, m2alloc, buffer, meta_node.ptr_hdList);
				hashtable_entry_add(m2mem, m2alloc, buffer, ipack.ptr_hashtable, meta_node.pair);
			}
			opack.ptr_list = NULL_PTR;
		}
	}else if(ipack.key_valid == 0){
		opack.key_valid = 1;
		// data should be key
		opack.key = data;
	}else{
		// data should be value
		// got a new key-value pair, add to list
		struct pair_t pair;
		pair.key = ipack.key;
		pair.val = data;
		opack.ptr_list = list_node_add(m2mem, m2alloc, buffer, ipack.ptr_list, pair);
	}
	return opack;
}

// print out
void print_table(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr){
	//printf("\n\nPrinting table...\n");
	for(int i = 0; i < HASHTABLE_NUM_BUCKETS; i++){
		//printf("bucket(%d): \n", i);
		ptr_t ptr2bucket = hashtable_bucket_get(m2mem, m2alloc, buffer, ptr, i);
		while(ptr2bucket != NULL_PTR){
			struct bucket_node_t bucket_node = bucket_node_read(m2mem, m2alloc, buffer, ptr2bucket);
			ptr2bucket = bucket_node.ptr_next;
			//printf("[%lu] -> [", bucket_node.key);
			ptr_t ptr2vals = bucket_node.ptr_vals;
			while(ptr2vals != NULL_PTR){
				struct vals_node_t valNode = vals_node_read(m2mem, m2alloc, buffer, ptr2vals);
				ptr2vals = valNode.ptr_next;
				//printf(" %lu", valNode.val);
			}
			//printf("]\n");
		}
	}	
}