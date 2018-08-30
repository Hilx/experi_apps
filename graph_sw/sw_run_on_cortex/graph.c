#include "graph.h"

// graph operations
void graph_addedge(ptr_t ptr_root, data_t key0, data_t key1){
	/*
	print("adding egde pair (");
	putnum(key0);
	print(" ");
	putnum(key1);
	print(")...\n\r");
	*/
	if(key0 != key1){ // a node cannot has a link to itself
		// acquire graph root
		struct root_t graph_root = root_read(ptr_root);
		if(graph_root.key_cnt == 0){ // NULL GRAPH
			ptr_t ptr_bucket_node0 = bucket_node_create(NULL_PTR, key0, key1);
			ptr_t ptr_bucket_node1 = bucket_node_create(NULL_PTR, key1, key0);
			data_t hash_id0 = hashing(key0);
			data_t hash_id1 = hashing(key1);
			ptr_t ptr_hashtable = graph_root.ptr_hashtable;
			hashtable_update(ptr_hashtable, hash_id0, ptr_bucket_node0);
			hashtable_update(ptr_hashtable, hash_id1, ptr_bucket_node1);
			graph_root.key_cnt = 2;
			root_write(ptr_root, graph_root);
		}else{ // GRAPH EXISTS
			// check if key0 already exists in graph
			ptr_t ptr_bucket = bucket_ptr_get(graph_root.ptr_hashtable, key0);
			if(ptr_bucket == NULL_PTR){
				ptr_t ptr_bucket_local = bucket_node_create(NULL_PTR, key0, key1);
				data_t hash_id = hashing(key0);
				hashtable_update(graph_root.ptr_hashtable, hash_id, ptr_bucket_local);
				graph_root.key_cnt = graph_root.key_cnt + 1;
				root_write(ptr_root, graph_root);				
				second_key_process(ptr_root, graph_root, key1, key0);				
			}else{
				struct bucket_node_pair_t bucket_node_pair;
				bucket_node_pair = check_key(ptr_bucket, key0);
				if(bucket_node_pair.bucket_node_curr.key == key0){ // key0 exists
					// check if link exists
					struct keyset_node_pair_t keyset_node_pair;
					keyset_node_pair = check_link(bucket_node_pair.bucket_node_curr.ptr_keyset, key1);
					if(keyset_node_pair.keyset_node_curr.key == key1){ // link already exists
						// NULL;
					}else{ // link doesn't exist
						// create link key0 to key1 in key0's keyset
						ptr_t ptr_keyset_local = create_link(bucket_node_pair.bucket_node_curr.ptr_keyset, keyset_node_pair, key1);
						struct bucket_node_t bucket_node2update;
						bucket_node2update.ptr_next = bucket_node_pair.bucket_node_curr.ptr_next;
						bucket_node2update.ptr_keyset = ptr_keyset_local;
						bucket_node2update.link_cnt = bucket_node_pair.bucket_node_curr.link_cnt + 1;
						bucket_node2update.key = key0;
						bucket_node_write(bucket_node_pair.ptr_node_curr, bucket_node2update);
						//graph_root.key_cnt = graph_root.key_cnt;// + 1;
						//root_write(ptr_root, graph_root);
						second_key_process(ptr_root, graph_root, key1, key0);
					}
				}else{ // key0 doesn't exist
					ptr_t ptr_bucket_local = add_bucket_node(ptr_bucket, bucket_node_pair, key0, key1);
					if(ptr_bucket_local != ptr_bucket){
						// update ptr2bucket in hashtable
						data_t hash_id = hashing(key0);
						hashtable_update(graph_root.ptr_hashtable, hash_id, ptr_bucket_local);
					}
					graph_root.key_cnt = graph_root.key_cnt + 1;
					root_write(ptr_root, graph_root);				
					second_key_process(ptr_root, graph_root, key1, key0);
				}
			}
		}
	}
}

//
struct bucket_node_pair_t check_key(ptr_t ptr_bucket, data_t key){
	struct bucket_node_t node;
	struct bucket_node_t node_prev;
	ptr_t ptr_curr = ptr_bucket;
	ptr_t ptr_prev = NULL_PTR;
	node = bucket_node_read(ptr_bucket);
	while(node.key < key && node.ptr_next != NULL_PTR){
		node_prev = node;
		ptr_prev = ptr_curr;
		ptr_curr = node.ptr_next;
		node = bucket_node_read(node.ptr_next);
	}
	struct bucket_node_pair_t node_pair;
	node_pair.ptr_node_prev = ptr_prev;
	node_pair.ptr_node_curr = ptr_curr;
	node_pair.bucket_node_prev = node_prev;
	node_pair.bucket_node_curr = node;
	return node_pair;
}
struct keyset_node_pair_t check_link(ptr_t ptr_keyset, data_t linking_key){
	struct keyset_node_t node;
	struct keyset_node_t node_prev;
	ptr_t ptr_curr = ptr_keyset;
	ptr_t ptr_prev = NULL_PTR;
	node = keyset_node_read(ptr_keyset);
	while(node.key < linking_key && node.ptr_next != NULL_PTR){
		node_prev = node;
		ptr_prev = ptr_curr;
		ptr_curr = node.ptr_next;
		node = keyset_node_read(node.ptr_next);
	}
	struct keyset_node_pair_t node_pair;
	node_pair.ptr_node_prev = ptr_prev;
	node_pair.ptr_node_curr = ptr_curr;
	node_pair.keyset_node_prev = node_prev;
	node_pair.keyset_node_curr = node;
	return node_pair;
}

void second_key_process(ptr_t ptr_root, struct root_t graph_root, data_t key1, data_t linking_key){
	//printf("processing second key (%lu) linking key = %lu\n", key1, linking_key);
	ptr_t ptr_bucket = bucket_ptr_get(graph_root.ptr_hashtable, key1);
	if(ptr_bucket == NULL_PTR){
		ptr_t ptr_bucket_local = bucket_node_create(NULL_PTR, key1, linking_key);
		data_t hash_id = hashing(key1);
		hashtable_update(graph_root.ptr_hashtable, hash_id, ptr_bucket_local);		
		graph_root.key_cnt = graph_root.key_cnt + 1;
		root_write(ptr_root, graph_root);
	}else{
		struct bucket_node_pair_t bucket_node_pair = check_key(ptr_bucket, key1);
		if(bucket_node_pair.bucket_node_curr.key == key1){ // key1 exists
			// create link key1 to key0 in key1's keyset
			struct keyset_node_pair_t keyset_node_pair = check_link(bucket_node_pair.bucket_node_curr.ptr_keyset, linking_key);
			ptr_t ptr_keyset_local = create_link(bucket_node_pair.bucket_node_curr.ptr_keyset, keyset_node_pair, linking_key);				
			struct bucket_node_t bucket_node2update;
			bucket_node2update.ptr_next = bucket_node_pair.bucket_node_curr.ptr_next;
			bucket_node2update.ptr_keyset = ptr_keyset_local;
			bucket_node2update.link_cnt = bucket_node_pair.bucket_node_curr.link_cnt + 1;
			bucket_node2update.key = key1;
			bucket_node_write(bucket_node_pair.ptr_node_curr, bucket_node2update);
		}else{ // key1 doesn't exist
			ptr_t ptr_bucket_local = add_bucket_node(ptr_bucket, bucket_node_pair, key1, linking_key);
			if(ptr_bucket_local != ptr_bucket){
				// update ptr2bucket in hashtable
				data_t hash_id = hashing(key1);
				hashtable_update(graph_root.ptr_hashtable, hash_id, ptr_bucket_local);
			}
			graph_root.key_cnt = graph_root.key_cnt + 1;
			root_write(ptr_root, graph_root);		
		}	
	}		
}

/* ------------ Software Only Function ---------------- */
void graph_printout(ptr_t ptr2graph){
	struct root_t graph_root = root_read(ptr2graph);
	/*
	print("\nPrinting Graph...\n");
	print("Number of Keys = ");
	putnum(graph_root.key_cnt);
	print(", ptr2hashtable = ");
	putnum(graph_root.ptr_hashtable);
	print(" \n\r");
	*/
	int i;
	for(i = 0; i < HASHTABLE_NUM_BUCKETS; i++){
		//printf("bucket(%d): \n", i);
		ptr_t ptr2bucket = bucket_ptr_get(graph_root.ptr_hashtable, i);
		while(ptr2bucket != NULL_PTR){
			struct bucket_node_t bucket_node = bucket_node_read(ptr2bucket);
			ptr2bucket = bucket_node.ptr_next;
			/*
			print("[");
			putnum(bucket_node.key);
			print("] (");
			putnum(bucket_node.link_cnt);
			print(" links) -> [");
			*/
			ptr_t ptr_keyset = bucket_node.ptr_keyset;
			int j;
			for(j = 0; j < bucket_node.link_cnt; j++){
				struct keyset_node_t keyset_node = keyset_node_read(ptr_keyset);
				ptr_keyset = keyset_node.ptr_next;
				//putnum(keyset_node.key);
			}
			//print("]\n");
		}
	}

	//printf("------------------------------ \n\n\n\n\n");
}
