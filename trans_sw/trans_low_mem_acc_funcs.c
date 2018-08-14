#include "trans.h"

// in-trann. list operations // unsorted list
struct list_node_t list_node_read(ptr_t ptr){
	data_t * ptr_i;
	ptr_i = ptr;
	struct list_node_t node;
	node.ptr_next = *(ptr_i + LIST_OFFSET_PTR_NEXT);
	node.pair.key = *(ptr_i + LIST_OFFSET_KEY);
	node.pair.val = *(ptr_i + LIST_OFFSET_VAL);
	return node;
}
void list_node_write(ptr_t ptr, struct list_node_t node){
	data_t * ptr_i;
	ptr_i = ptr;
	*(ptr_i + LIST_OFFSET_PTR_NEXT) = node.ptr_next;
	*(ptr_i + LIST_OFFSET_KEY) = node.pair.key;
	*(ptr_i + LIST_OFFSET_VAL) = node.pair.val;
}
// hashtable oprations
ptr_t hashtable_bucket_get(ptr_t ptr, data_t key){
	data_t * ptr_i;
	ptr_i = ptr;
	data_t hashID = hashing(key);
	ptr_t ptr_bucket = *(ptr_i + hashID);
	return ptr_bucket;
}
void hashtable_bucket_update(ptr_t ptr, data_t key, ptr_t ptr_bucket){
	data_t * ptr_i;
	ptr_i = ptr;
	data_t hashID = hashing(key);
	*(ptr_i + hashID) = ptr_bucket;	
}
// bucket operations // sorted list
struct bucket_node_t bucket_node_read(ptr_t ptr){
	data_t * ptr_i;
	ptr_i = ptr;
	struct bucket_node_t node;
	node.ptr_next = *(ptr_i + BUCK_OFFSET_PTR_NEXT);
	node.ptr_vals = *(ptr_i + BUCK_OFFSET_PTR_VALS);
	node.key = *(ptr_i + BUCK_OFFSET_KEY);
	return node;
}
void bucket_node_write(ptr_t ptr, struct bucket_node_t node){
	data_t * ptr_i;
	ptr_i = ptr;
	*(ptr_i + BUCK_OFFSET_PTR_NEXT) = node.ptr_next;
	*(ptr_i + BUCK_OFFSET_PTR_VALS) = node.ptr_vals;
	*(ptr_i + BUCK_OFFSET_KEY) = node.key;
}
// vals operations // unsorted list
struct vals_node_t vals_node_read(ptr_t ptr){
	data_t * ptr_i;
	ptr_i = ptr;
	struct vals_node_t node;
	node.ptr_next = *(ptr_i + VALS_OFFSET_PTR_NEXT);
	node.val = *(ptr_i + VALS_OFFSET_VAL);
	return node;
}
void vals_node_write(ptr_t ptr, struct vals_node_t node){
	data_t * ptr_i;
	ptr_i = ptr;
	*(ptr_i + VALS_OFFSET_PTR_NEXT) = node.ptr_next;
	*(ptr_i + VALS_OFFSET_VAL) = node.val;
}