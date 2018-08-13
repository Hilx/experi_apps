#include "malloc_wrapper.h"

#define NULL_PTR 0xFFFF0007 // null value
#define HASHTABLE_NUM_BUCKETS 10 // Configuration
// OFFSETS 
#define ROOT_OFFSET_KEY_CNT 			0
#define ROOT_OFFSET_PTR_HASHTABLE 		1
#define BUCKET_NODE_OFFSET_PTR_NEXT 	0
#define BUCKET_NODE_OFFSET_PTR_KEYSET 	1
#define BUCKET_NODE_OFFSET_LINK_CNT 	2
#define BUCKET_NODE_OFFSET_KEY 			3
#define KEYSET_NODE_OFFSET_PTR_NEXT 	0
#define KEYSET_NODE_OFFSET_KEY 			1

// structs
struct root_t {
	data_t key_cnt;
	ptr_t ptr_hashtable;
};
struct hashtable_entry_t{
	ptr_t ptr_bucket;
};
struct bucket_node_t{
	ptr_t ptr_next;
	ptr_t ptr_keyset;
	data_t link_cnt;
	data_t key;
};
struct bucket_node_pair_t{
	ptr_t ptr_node_prev;
	ptr_t ptr_node_curr;
	struct bucket_node_t bucket_node_prev;
	struct bucket_node_t bucket_node_curr;
};
struct keyset_node_t{
	ptr_t ptr_next;
	data_t key;
};
struct keyset_node_pair_t{
	ptr_t ptr_node_prev;
	ptr_t ptr_node_curr;
	struct keyset_node_t keyset_node_prev;
	struct keyset_node_t keyset_node_curr;
};

// root operations
ptr_t root_create(data_t * m2mem, data_t * m2alloc, data_t * buffer);
struct root_t root_read(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr_root);
void root_write(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr_root, struct root_t graph_root);
// hash table operations
ptr_t hashtable_create(data_t * m2mem, data_t * m2alloc, data_t * buffer);
data_t hashing(data_t key);
void hashtable_update(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr_hashtable, data_t hash_id, ptr_t ptr_bucket_node);
ptr_t bucket_ptr_get(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr_hashtable, data_t key);
// bucket operations
ptr_t bucket_node_create(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr_next_bucket_node, data_t key, data_t linking_key);
struct bucket_node_t bucket_node_read(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr_bucket_node);
void bucket_node_write(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr_bucket_node, struct bucket_node_t node);
// keyset operations
ptr_t keyset_node_create(data_t * m2mem, data_t * m2alloc, data_t * buffer, struct keyset_node_t keyset_node);
struct keyset_node_t keyset_node_read(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr_keyset_node);
void keyset_node_write(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr_keyset_node, struct keyset_node_t keyset_node);
// graph operations
void graph_addedge(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr_root, data_t key0, data_t key1);
struct bucket_node_pair_t check_key(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr_bucket, data_t key);
struct keyset_node_pair_t check_link(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr_keyset, data_t linking_key);
ptr_t add_bucket_node(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr_bucket, struct bucket_node_pair_t node_pair, data_t key, data_t linking_key);
ptr_t create_link(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr_keyset, struct keyset_node_pair_t node_pair, data_t linking_key);
void second_key_process(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr_root, struct root_t graph_root, data_t key1, data_t linking_key);

/* ------------ Software Only Function ---------------- */
data_t graph_printout(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr2graph);