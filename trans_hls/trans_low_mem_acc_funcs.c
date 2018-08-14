#include "trans.h"

// in-trann. list operations // unsorted list
struct list_node_t list_node_read(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr){
	memcpy(buffer, (data_t *)(m2mem + ptr/4), sizeof(struct list_node_t);
	struct list_node_t node;
	node.ptr_next = buffer[LIST_OFFSET_PTR_NEXT];
	node.pair.key = buffer[LIST_OFFSET_KEY];
	node.pair.val = buffer[LIST_OFFSET_VAL];
	return node;
}
void list_node_write(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr, struct list_node_t node){
	buffer[LIST_OFFSET_PTR_NEXT] = node.ptr_next;
	buffer[LIST_OFFSET_KEY] = node.pair.key;
	buffer[LIST_OFFSET_VAL] = node.pair.val;
	memcpy((data_t *)(m2mem + ptr/4), buffer, sizeof(struct list_node_t);
}
// hashtable oprations
ptr_t hashtable_bucket_get(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr, data_t key){
	data_t hashID = hashing(key);
	ptr_t ptr_bucket = m2mem[ptr/4 + hashID];
	return ptr_bucket;
}
void hashtable_bucket_update(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr, data_t key, ptr_t ptr_bucket){
	data_t hashID = hashing(key);
	m2mem[ptr/4 + hashID] = ptr_bucket;	
}
// bucket operations // sorted list
struct bucket_node_t bucket_node_read(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr){
	memcpy(buffer, (data_t *)(m2mem + ptr/4), sizeof(struct bucket_node_t);
	struct bucket_node_t node;
	node.ptr_next = buffer[BUCK_OFFSET_PTR_NEXT];
	node.ptr_vals = buffer[BUCK_OFFSET_PTR_VALS];
	node.key = buffer[BUCK_OFFSET_KEY];
	return node;
}
void bucket_node_write(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr, struct bucket_node_t node){
	buffer[BUCK_OFFSET_PTR_NEXT]= node.ptr_next;
	buffer[BUCK_OFFSET_PTR_VALS] = node.ptr_vals;
	buffer[BUCK_OFFSET_KEY] = node.key;
	memcpy((data_t *)(m2mem + ptr/4), buffer, sizeof(struct bucket_node_t);
}
// vals operations // unsorted list
struct vals_node_t vals_node_read(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr){
	memcpy(buffer, (data_t *)(m2mem + ptr/4), sizeof(struct vals_node_t);
	struct vals_node_t node;
	node.ptr_next = buffer[VALS_OFFSET_PTR_NEXT];
	node.val = buffer[VALS_OFFSET_VAL];
	return node;
}
void vals_node_write(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr, struct vals_node_t node){
	buffer[VALS_OFFSET_PTR_NEXT] = node.ptr_next;
	buffer[VALS_OFFSET_VAL] = node.val;
	memcpy((data_t *)(m2mem + ptr/4), buffer, sizeof(struct vals_node_t);
}