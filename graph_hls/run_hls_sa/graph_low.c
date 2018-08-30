#include "graph.h"

// root operations
ptr_t root_create(data_t * m2mem, data_t * m2alloc, data_t * buffer){
	//printf("root creation\n");
	// create hash table 
	ptr_t ptr_hashtable = hashtable_create(m2mem, m2alloc, buffer);
	// initial root content
    struct root_t graph_root;
    graph_root.key_cnt = 0;
    graph_root.ptr_hashtable = ptr_hashtable;
    // allocate memory for root and write initial content in
    //ptr_t ptr_root = custom_malloc(m2mem, m2alloc, sizeof(struct root_t));
	ptr_t ptr_root = custom_malloc(m2mem, m2alloc,  8);
    root_write(m2mem, m2alloc, buffer, ptr_root, graph_root);
    return ptr_root;
}

data_t hashing(data_t key){
	data_t hash_id = key % HASHTABLE_NUM_BUCKETS;
	return key % HASHTABLE_NUM_BUCKETS;
}

// bucket operations
ptr_t bucket_node_create(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr_next_bucket_node, data_t key, data_t linking_key){
	ptr_t ptr_bucket = custom_malloc(m2mem, m2alloc, 16);
	//printf("        Creating bucket node for key [%d], bucket node ptr is %d\n", key, ptr_bucket);
	struct keyset_node_t keyset_node;
	keyset_node.ptr_next = NULL_PTR;
	keyset_node.key = linking_key;
	ptr_t ptr_keyset = keyset_node_create(m2mem, m2alloc, buffer, keyset_node);
	struct bucket_node_t bucket_node;
	bucket_node.ptr_next = ptr_next_bucket_node;
	bucket_node.ptr_keyset = ptr_keyset;
	bucket_node.link_cnt = 1;
	bucket_node.key = key;
	bucket_node_write(m2mem, m2alloc, buffer, ptr_bucket, bucket_node);
	return ptr_bucket;
}

// keyset operations
ptr_t keyset_node_create(data_t * m2mem, data_t * m2alloc, data_t * buffer, struct keyset_node_t keyset_node){
	ptr_t ptr_keyset_node = custom_malloc(m2mem, m2alloc, 8);
	keyset_node_write(m2mem, m2alloc, buffer, ptr_keyset_node, keyset_node);
	return ptr_keyset_node;
}

// 
ptr_t add_bucket_node(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr_bucket, struct bucket_node_pair_t node_pair, data_t key, data_t linking_key){
	ptr_t ptr2return = ptr_bucket;
	if(key < node_pair.bucket_node_curr.key){
		ptr_t ptr_new = bucket_node_create(m2mem, m2alloc, buffer, node_pair.ptr_node_curr, key, linking_key);
		if(node_pair.ptr_node_prev == NULL_PTR){
			ptr2return = ptr_new;	
		}else{
			struct bucket_node_t prev;
			prev.ptr_next = ptr_new;
			prev.ptr_keyset = node_pair.bucket_node_prev.ptr_keyset;
			prev.link_cnt = node_pair.bucket_node_prev.link_cnt;
			prev.key = node_pair.bucket_node_prev.key;
			bucket_node_write(m2mem, m2alloc, buffer, node_pair.ptr_node_prev, prev);
		}
	}else{
		ptr_t ptr_new = bucket_node_create(m2mem, m2alloc, buffer, node_pair.bucket_node_curr.ptr_next, key, linking_key);
		struct bucket_node_t bucket_node_update;
		bucket_node_update.ptr_next = ptr_new;
		bucket_node_update.ptr_keyset = node_pair.bucket_node_curr.ptr_keyset;
		bucket_node_update.link_cnt = node_pair.bucket_node_curr.link_cnt;
		bucket_node_update.key = node_pair.bucket_node_curr.key;
		bucket_node_write(m2mem, m2alloc, buffer, node_pair.ptr_node_curr, bucket_node_update);
	}
	return ptr2return;
}

ptr_t create_link(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr_keyset, struct keyset_node_pair_t node_pair, data_t linking_key){
	// return the ptr_keyset unless the new link takes place of the head
	// malloc space of new key
	ptr_t ptr2return = ptr_keyset;
	if(linking_key < node_pair.keyset_node_curr.key){
		struct keyset_node_t keyset_node_new;
		keyset_node_new.ptr_next = node_pair.ptr_node_curr;
		keyset_node_new.key = linking_key;
		ptr_t ptr_new = keyset_node_create(m2mem, m2alloc, buffer, keyset_node_new);	
		// check for prev existence
		if(node_pair.ptr_node_prev == NULL_PTR){
			ptr2return = ptr_new;
		}else{
			struct keyset_node_t prev;
			prev.ptr_next = ptr_new;
			prev.key = node_pair.keyset_node_prev.key;
			keyset_node_write(m2mem, m2alloc, buffer, node_pair.ptr_node_prev, prev);
		}
	}else{
		struct keyset_node_t keyset_node_new;
		keyset_node_new.ptr_next = node_pair.keyset_node_curr.ptr_next;
		keyset_node_new.key = linking_key;
		ptr_t ptr_new = keyset_node_create(m2mem, m2alloc, buffer, keyset_node_new);
		struct keyset_node_t keyset_node_update;
		keyset_node_update.ptr_next = ptr_new;
		keyset_node_update.key = node_pair.keyset_node_curr.key;
		keyset_node_write(m2mem, m2alloc, buffer, node_pair.ptr_node_curr, keyset_node_update);
	}	
	return ptr2return;
}
