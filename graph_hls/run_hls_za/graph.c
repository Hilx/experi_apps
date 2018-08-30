#include "graph.h"

// graph operations
volatile void graph_addedge(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr_root, data_t key0, data_t key1){
	//printf("   Adding Edge (%d, %d)...\n", key0, key1);
	if(key0 != key1){ // a node cannot has a link to itself
		// acquire graph root
		struct root_t graph_root = root_read(m2mem, m2alloc, buffer, ptr_root);
		if(graph_root.key_cnt == 0){ // NULL GRAPH
			ptr_t ptr_bucket_node0 = bucket_node_create(m2mem, m2alloc, buffer, NULL_PTR, key0, key1);
			ptr_t ptr_bucket_node1 = bucket_node_create(m2mem, m2alloc, buffer, NULL_PTR, key1, key0);
			ptr_t ptr_hashtable = graph_root.ptr_hashtable;
			hashtable_update(m2mem, m2alloc, buffer, ptr_hashtable, hashing(key0), ptr_bucket_node0);
			hashtable_update(m2mem, m2alloc, buffer, ptr_hashtable, hashing(key1), ptr_bucket_node1);
			graph_root.key_cnt = 2;
			root_write(m2mem, m2alloc, buffer, ptr_root, graph_root);
		}else{ // GRAPH EXISTS
			// check if key0 already exists in graph
			ptr_t ptr_bucket = bucket_ptr_get(m2mem, m2alloc, buffer, graph_root.ptr_hashtable, key0);
			if(ptr_bucket == NULL_PTR){ // key0 doesn't exist
				//printf("   key0 [%d] doesn't exist\n", key0);
				ptr_t ptr_bucket_local = bucket_node_create(m2mem, m2alloc, buffer, NULL_PTR, key0, key1);
				hashtable_update(m2mem, m2alloc, buffer, graph_root.ptr_hashtable, hashing(key0), ptr_bucket_local);
				graph_root.key_cnt = graph_root.key_cnt + 1;
				root_write(m2mem, m2alloc, buffer, ptr_root, graph_root);				
				second_key_process(m2mem, m2alloc, buffer, ptr_root, graph_root, key1, key0);
			}else{ // key0 exists
				//printf("   key0 [%d] exists\n", key0);
				struct bucket_node_pair_t bucket_node_pair;
				bucket_node_pair = check_key(m2mem, m2alloc, buffer, ptr_bucket, key0);
				if(bucket_node_pair.bucket_node_curr.key == key0){ // key0 exists
					// check if link exists
					struct keyset_node_pair_t keyset_node_pair;
					keyset_node_pair = check_link(m2mem, m2alloc, buffer, bucket_node_pair.bucket_node_curr.ptr_keyset, key1);
					if(keyset_node_pair.keyset_node_curr.key == key1){ // link already exists
						// NULL;
					}else{ // link doesn't exist
						// create link key0 to key1 in key0's keyset
						ptr_t ptr_keyset_local = create_link(m2mem, m2alloc, buffer, bucket_node_pair.bucket_node_curr.ptr_keyset, keyset_node_pair, key1);
						struct bucket_node_t bucket_node2update;
						bucket_node2update.ptr_next = bucket_node_pair.bucket_node_curr.ptr_next;
						bucket_node2update.ptr_keyset = ptr_keyset_local;
						bucket_node2update.link_cnt = bucket_node_pair.bucket_node_curr.link_cnt + 1;
						bucket_node2update.key = key0;
						bucket_node_write(m2mem, m2alloc, buffer, bucket_node_pair.ptr_node_curr, bucket_node2update);
						second_key_process(m2mem, m2alloc, buffer, ptr_root, graph_root, key1, key0);
					}
				}else{ // key0 doesn't exist
					ptr_t ptr_bucket_local = add_bucket_node(m2mem, m2alloc, buffer, ptr_bucket, bucket_node_pair, key0, key1);
					if(ptr_bucket_local != ptr_bucket){
						// update ptr2bucket in hashtable
						hashtable_update(m2mem, m2alloc, buffer, graph_root.ptr_hashtable,  hashing(key0), ptr_bucket_local);
					}
					graph_root.key_cnt = graph_root.key_cnt + 1;
					root_write(m2mem, m2alloc, buffer, ptr_root, graph_root);				
					second_key_process(m2mem, m2alloc, buffer, ptr_root, graph_root, key1, key0);
				}
			}
		}
	}
}
struct bucket_node_pair_t check_key(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr_bucket, data_t key){
	struct bucket_node_t node;
	struct bucket_node_t node_prev;
	ptr_t ptr_curr = ptr_bucket;
	ptr_t ptr_prev = NULL_PTR;

	node = bucket_node_read(m2mem, m2alloc, buffer, ptr_bucket);
	while(node.key < key && node.ptr_next != NULL_PTR){
		node_prev = node;
		ptr_prev = ptr_curr;
		ptr_curr = node.ptr_next;
		node = bucket_node_read(m2mem, m2alloc, buffer, node.ptr_next);
	}
	struct bucket_node_pair_t node_pair;
	node_pair.ptr_node_prev = ptr_prev;
	node_pair.ptr_node_curr = ptr_curr;
	node_pair.bucket_node_prev = node_prev;
	node_pair.bucket_node_curr = node;
	return node_pair;
}
struct keyset_node_pair_t check_link(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr_keyset, data_t linking_key){
	struct keyset_node_t node;
	struct keyset_node_t node_prev;
	ptr_t ptr_curr = ptr_keyset;
	ptr_t ptr_prev = NULL_PTR;
	node = keyset_node_read(m2mem, m2alloc, buffer, ptr_keyset);
	while(node.key < linking_key && node.ptr_next != NULL_PTR){
		node_prev = node;
		ptr_prev = ptr_curr;
		ptr_curr = node.ptr_next;
		node = keyset_node_read(m2mem, m2alloc, buffer, node.ptr_next);
	}
	struct keyset_node_pair_t node_pair;
	node_pair.ptr_node_prev = ptr_prev;
	node_pair.ptr_node_curr = ptr_curr;
	node_pair.keyset_node_prev = node_prev;
	node_pair.keyset_node_curr = node;
	return node_pair;
}

volatile void second_key_process(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr_root, struct root_t graph_root, data_t key1, data_t linking_key){
	//printf("   Processing key1 [%d]\n", key1);
	ptr_t ptr_bucket = bucket_ptr_get(m2mem, m2alloc, buffer, graph_root.ptr_hashtable, key1);
	//printf("   -- BucketPtr = %d\n");
	if(ptr_bucket == NULL_PTR){
		//printf("       Empty Bucket...\n");
		ptr_t ptr_bucket_local = bucket_node_create(m2mem, m2alloc, buffer, NULL_PTR, key1, linking_key);
		hashtable_update(m2mem, m2alloc, buffer, graph_root.ptr_hashtable, hashing(key1), ptr_bucket_local);
		graph_root.key_cnt = graph_root.key_cnt + 1;
		root_write(m2mem, m2alloc, buffer, ptr_root, graph_root);
	}else{
		struct bucket_node_pair_t bucket_node_pair = check_key(m2mem, m2alloc, buffer, ptr_bucket, key1);
		if(bucket_node_pair.bucket_node_curr.key == key1){ // key1 exists
			//printf("       key1 exists\n");
			// create link key1 to key0 in key1's keyset
			struct keyset_node_pair_t keyset_node_pair = check_link(m2mem, m2alloc, buffer, bucket_node_pair.bucket_node_curr.ptr_keyset, linking_key);
			ptr_t ptr_keyset_local = create_link(m2mem, m2alloc, buffer, bucket_node_pair.bucket_node_curr.ptr_keyset, keyset_node_pair, linking_key);
			struct bucket_node_t bucket_node2update;
			bucket_node2update.ptr_next = bucket_node_pair.bucket_node_curr.ptr_next;
			bucket_node2update.ptr_keyset = ptr_keyset_local;
			bucket_node2update.link_cnt = bucket_node_pair.bucket_node_curr.link_cnt + 1;
			bucket_node2update.key = key1;
			bucket_node_write(m2mem, m2alloc, buffer, bucket_node_pair.ptr_node_curr, bucket_node2update);
		}else{ // key1 doesn't exist
			//printf("       key1 doesn't exist\n");
			ptr_t ptr_bucket_local = add_bucket_node(m2mem, m2alloc, buffer, ptr_bucket, bucket_node_pair, key1, linking_key);
			if(ptr_bucket_local != ptr_bucket){
				// update ptr2bucket in hashtable
				hashtable_update(m2mem, m2alloc, buffer, graph_root.ptr_hashtable,  hashing(key1), ptr_bucket_local);
			}
			graph_root.key_cnt = graph_root.key_cnt + 1;
			root_write(m2mem, m2alloc, buffer, ptr_root, graph_root);		
		}	
	}		
}

/* ------------ Software Only Function ---------------- */
data_t graph_printout(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr2graph){
	//printf("\n\n PRINTING STARTS HERE \n");
	//printf(" ------------------------ \n");
	//printf(" ------------------------ \n");
	data_t key_cnt = 0;
	struct root_t graph_root = root_read(m2mem, m2alloc, buffer, ptr2graph);
	int i;
	for(i = 0; i < HASHTABLE_NUM_BUCKETS; i++){

		ptr_t ptr2bucket = bucket_ptr_get(m2mem, m2alloc, buffer, graph_root.ptr_hashtable, i);
		//if(ptr2bucket != NULL_PTR){
			//printf(">> Hash Bucket No.%d\n", i);
			//printf("   bucketPtr = %d\n", ptr2bucket);
		//}
		while(ptr2bucket != NULL_PTR){
			struct bucket_node_t bucket_node = bucket_node_read(m2mem, m2alloc, buffer, ptr2bucket);
			ptr2bucket = bucket_node.ptr_next;
			ptr_t ptr_keyset = bucket_node.ptr_keyset;
			//printf("   Key in bucket = %d, link cnt = %d\n",bucket_node.key, bucket_node.link_cnt);
			int j;
			//printf("LINKCNT WHILE PRINTING = %d\n",bucket_node.link_cnt);
			while(ptr_keyset != NULL_PTR){
			//for(j = 0; j < bucket_node.link_cnt; j++){
				struct keyset_node_t keyset_node = keyset_node_read(m2mem, m2alloc, buffer, ptr_keyset);
				ptr_keyset = keyset_node.ptr_next;
			}
			key_cnt++;
		}
	}
	return key_cnt;
}
