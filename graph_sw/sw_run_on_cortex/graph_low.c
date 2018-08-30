#include "graph.h"

// root operations
ptr_t root_create(void){
	// create hash table 
	ptr_t ptr_hashtable = hashtable_create();
	// initial root content
    struct root_t graph_root;
    graph_root.key_cnt = 0;
    graph_root.ptr_hashtable = ptr_hashtable;
    // allocate memory for root and write initial content in
    ptr_t ptr_root = custom_malloc(sizeof(struct root_t));	
    root_write(ptr_root, graph_root);
    return ptr_root;
}
struct root_t root_read(ptr_t ptr_root){
	data_t * ptr;
	ptr = ptr_root;
	struct root_t graph_root;
	graph_root.key_cnt = *(ptr + ROOT_OFFSET_KEY_CNT);
	graph_root.ptr_hashtable = *(ptr + ROOT_OFFSET_PTR_HASHTABLE);
	return graph_root;
}
void root_write(ptr_t ptr_root, struct root_t graph_root){
	data_t  * ptr;
	ptr = ptr_root;
	*(ptr + ROOT_OFFSET_KEY_CNT) = graph_root.key_cnt;
	*(ptr + ROOT_OFFSET_PTR_HASHTABLE) = graph_root.ptr_hashtable;
}

// hash table operations
ptr_t hashtable_create(void){
	// allocate memory for hash table all entries
	ptr_t ptr_hashtable = custom_malloc(sizeof(struct hashtable_entry_t) * HASHTABLE_NUM_BUCKETS);
	// initialise all ptrs to buckets to NULL
	data_t * ptr;
	ptr = ptr_hashtable;
	int i;
	for(i = 0; i < HASHTABLE_NUM_BUCKETS; i++){
		*(ptr + i) = NULL_PTR;
	}
	return ptr_hashtable;
}
data_t hashing(data_t key){
	return key % HASHTABLE_NUM_BUCKETS;
}
void hashtable_update(ptr_t ptr_hashtable, data_t hash_id, ptr_t ptr_bucket_node){
	data_t * ptr;
	ptr = ptr_hashtable;
	*(ptr + hash_id) = ptr_bucket_node;
}
ptr_t bucket_ptr_get(ptr_t ptr_hashtable, data_t key){
	data_t * ptr;
	ptr = ptr_hashtable;
	data_t hash_id = hashing(key);
	ptr_t ptr_bucket = *(ptr + hash_id);
	return ptr_bucket;
}

// bucket operations
ptr_t bucket_node_create(ptr_t ptr_next_bucket_node, data_t key, data_t linking_key){
	ptr_t ptr_bucket = custom_malloc(sizeof(struct bucket_node_t));
	struct keyset_node_t keyset_node;
	keyset_node.ptr_next = NULL_PTR;
	keyset_node.key = linking_key;
	ptr_t ptr_keyset = keyset_node_create(keyset_node);
	struct bucket_node_t bucket_node;
	bucket_node.ptr_next = ptr_next_bucket_node;
	bucket_node.ptr_keyset = ptr_keyset;
	bucket_node.link_cnt = 1;
	bucket_node.key = key;
	bucket_node_write(ptr_bucket, bucket_node);
	return ptr_bucket;
}
struct bucket_node_t bucket_node_read(ptr_t ptr_bucket_node){
	data_t * ptr;
	ptr = ptr_bucket_node;
	struct bucket_node_t bucket_node;
	bucket_node.ptr_next = *(ptr + BUCKET_NODE_OFFSET_PTR_NEXT);
	bucket_node.ptr_keyset = *(ptr + BUCKET_NODE_OFFSET_PTR_KEYSET);
	bucket_node.link_cnt = *(ptr + BUCKET_NODE_OFFSET_LINK_CNT);
	bucket_node.key = *(ptr + BUCKET_NODE_OFFSET_KEY);
	return bucket_node;
}
void bucket_node_write(ptr_t ptr_bucket_node, struct bucket_node_t bucket_node){
	data_t * ptr;
	ptr = ptr_bucket_node;
	*(ptr + BUCKET_NODE_OFFSET_PTR_NEXT) = bucket_node.ptr_next;
	*(ptr + BUCKET_NODE_OFFSET_PTR_KEYSET) = bucket_node.ptr_keyset;
	*(ptr + BUCKET_NODE_OFFSET_LINK_CNT) = bucket_node.link_cnt;
	*(ptr + BUCKET_NODE_OFFSET_KEY) = bucket_node.key;
}

// keyset operations
ptr_t keyset_node_create(struct keyset_node_t keyset_node){
	ptr_t ptr_keyset_node = custom_malloc(sizeof(struct keyset_node_t));
	keyset_node_write(ptr_keyset_node, keyset_node);
	return ptr_keyset_node;
}
struct keyset_node_t keyset_node_read(ptr_t ptr_keyset_node){
	data_t * ptr;
	ptr = ptr_keyset_node;
	struct keyset_node_t keyset_node;
	keyset_node.ptr_next = *(ptr + KEYSET_NODE_OFFSET_PTR_NEXT);
	keyset_node.key = *(ptr + KEYSET_NODE_OFFSET_KEY);
	return keyset_node;

}
void keyset_node_write(ptr_t ptr_keyset_node, struct keyset_node_t keyset_node){
	data_t * ptr;
	ptr = ptr_keyset_node;
	 *(ptr + KEYSET_NODE_OFFSET_PTR_NEXT) = keyset_node.ptr_next;
	 *(ptr + KEYSET_NODE_OFFSET_KEY) = keyset_node.key;
}

// 
ptr_t add_bucket_node(ptr_t ptr_bucket, struct bucket_node_pair_t node_pair, data_t key, data_t linking_key){
	ptr_t ptr2return = ptr_bucket;
	if(key < node_pair.bucket_node_curr.key){
		ptr_t ptr_new = bucket_node_create(node_pair.ptr_node_curr, key, linking_key);
		if(node_pair.ptr_node_prev == NULL_PTR){
			ptr2return = ptr_new;	
		}else{
			struct bucket_node_t prev;
			prev.ptr_next = ptr_new;
			prev.ptr_keyset = node_pair.bucket_node_prev.ptr_keyset;
			prev.link_cnt = node_pair.bucket_node_prev.link_cnt;
			prev.key = node_pair.bucket_node_prev.key;
			bucket_node_write(node_pair.ptr_node_prev, prev);
		}
	}else{
		ptr_t ptr_new = bucket_node_create(node_pair.bucket_node_curr.ptr_next, key, linking_key);
		struct bucket_node_t bucket_node_update;
		bucket_node_update.ptr_next = ptr_new;
		bucket_node_update.ptr_keyset = node_pair.bucket_node_curr.ptr_keyset;
		bucket_node_update.link_cnt = node_pair.bucket_node_curr.link_cnt;
		bucket_node_update.key = node_pair.bucket_node_curr.key;
		bucket_node_write(node_pair.ptr_node_curr, bucket_node_update);
	}
	return ptr2return;
}


ptr_t create_link(ptr_t ptr_keyset, struct keyset_node_pair_t node_pair, data_t linking_key){
	// return the ptr_keyset unless the new link takes place of the head
	// malloc space of new key
	ptr_t ptr2return = ptr_keyset;
	if(linking_key < node_pair.keyset_node_curr.key){
		struct keyset_node_t keyset_node_new;
		keyset_node_new.ptr_next = node_pair.ptr_node_curr;
		keyset_node_new.key = linking_key;
		ptr_t ptr_new = keyset_node_create(keyset_node_new);	
		// check for prev existence
		if(node_pair.ptr_node_prev == NULL_PTR){
			ptr2return = ptr_new;
		}else{
			struct keyset_node_t prev;
			prev.ptr_next = ptr_new;
			prev.key = node_pair.keyset_node_prev.key;
			keyset_node_write(node_pair.ptr_node_prev, prev);
		}
	}else{
		struct keyset_node_t keyset_node_new;
		keyset_node_new.ptr_next = node_pair.keyset_node_curr.ptr_next;
		keyset_node_new.key = linking_key;
		ptr_t ptr_new = keyset_node_create(keyset_node_new);
		struct keyset_node_t keyset_node_update;
		keyset_node_update.ptr_next = ptr_new;
		keyset_node_update.key = node_pair.keyset_node_curr.key;
		keyset_node_write(node_pair.ptr_node_curr, keyset_node_update);
	}	
	return ptr2return;
}
