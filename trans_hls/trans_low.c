#include "trans.h"

// in-trann. list operations // unsorted list
ptr_t list_node_add(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr, struct pair_t entry){
	ptr_t ptr_hdList = custom_malloc(m2mem, m2alloc, buffer, sizeof(struct list_node_t));
	struct list_node_t node;
	node.ptr_next = ptr;
	node.pair.key = entry.key;
	node.pair.val = entry.val;
	list_node_write(m2mem, m2alloc, buffer, ptr_hdList, node);
	return ptr_hdList;
}
struct list_node_removed_t list_node_remove(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr){
	struct list_node_removed_t returnSet;
	returnSet.ptr_hdList = ptr;
	if(ptr != NULL_PTR){
		struct list_node_t node;
		node = list_node_read(m2mem, m2alloc, buffer, ptr);
		returnSet.pair = node.pair;
		returnSet.ptr_hdList = node.ptr_next;
		custom_free(m2mem, m2alloc, buffer, ptr);
	}
	return returnSet;
}
void list_destroy(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr){
	ptr_t ptr_hdList = ptr;
	while(ptr_hdList != NULL_PTR){
		struct list_node_t node;
		node = list_node_read(m2mem, m2alloc, buffer, ptr_hdList);
		custom_free(m2mem, m2alloc, buffer, ptr_hdList);
		ptr_hdList = node.ptr_next;
	}
}
// hashtable oprations
data_t hashing(data_t key){
	return key % HASHTABLE_NUM_BUCKETS;
}
ptr_t hashtable_create(data_t * m2mem, data_t * m2alloc, data_t * buffer){
	ptr_t ptr = custom_malloc(m2mem, m2alloc, buffer, sizeof(data_t) * HASHTABLE_NUM_BUCKETS);
	for(int i = 0; i < HASHTABLE_NUM_BUCKETS; i++){
		hashtable_bucket_update(m2mem, m2alloc, buffer, ptr, i, NULL_PTR);
	}
	return ptr;
}
void hashtable_entry_add(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr, struct pair_t entry){
	ptr_t ptr_buck = hashtable_bucket_get(m2mem, m2alloc, buffer, ptr, entry.key);
	ptr_t ptr_new = bucket_node_add(m2mem, m2alloc, buffer, ptr_buck, entry);
	if(ptr_buck != ptr_new){
		hashtable_bucket_update(m2mem, m2alloc, buffer, ptr, entry.key, ptr_new);
	}
}
// bucket operations // sorted list
ptr_t bucket_node_create(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr_next, struct pair_t entry){
	ptr_t ptr_newValsNode = vals_node_add(m2mem, m2alloc, buffer, NULL_PTR, entry.val);
	ptr_t ptr_newBuckNode = custom_malloc(m2mem, m2alloc, buffer, sizeof(struct bucket_node_t));
	struct bucket_node_t node;
	node.ptr_next = ptr_next;
	node.ptr_vals = ptr_newValsNode;
	node.key = entry.key;	
	bucket_node_write(m2mem, m2alloc, buffer, ptr_newBuckNode,node);
	return ptr_newBuckNode;
}
ptr_t bucket_node_add(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr, struct pair_t entry){
	ptr_t ptr_hdList = ptr;
	if(ptr == NULL_PTR){
		ptr_t ptr_newBuckNode = bucket_node_create(m2mem, m2alloc, buffer, ptr, entry);
		ptr_hdList = ptr_newBuckNode;
	}else{ // have to search for a good spot for new node or existence check, if exists only add the val
		struct bucket_node_t currNode = bucket_node_read(m2mem, m2alloc, buffer, ptr);
		struct bucket_node_t prevNode;
		ptr_t ptr_currNode = ptr;
		ptr_t ptr_prevNode = NULL_PTR;
		while(currNode.key < entry.key && currNode.ptr_next != NULL_PTR){
			ptr_prevNode = ptr_currNode;
			prevNode = currNode;
			ptr_currNode = currNode.ptr_next;
			currNode = bucket_node_read(m2mem, m2alloc, buffer, currNode.ptr_next);
		}
		if(currNode.key == entry.key){ // bucket node already exists, don't create new bucket node
			ptr_t ptr_vals = vals_node_add(m2mem, m2alloc, buffer, currNode.ptr_vals, entry.val);
			currNode.ptr_vals = ptr_vals;
			bucket_node_write(m2mem, m2alloc, buffer, ptr, currNode);
		}else{ // bucket node not found, create new bucket node
			if(entry.key < currNode.key){
				ptr_t ptr_newBuckNode = bucket_node_create(m2mem, m2alloc, buffer, ptr_currNode, entry);
				if(ptr_prevNode == NULL_PTR){
					ptr_hdList = ptr_newBuckNode;
				}else{
					prevNode.ptr_next = ptr_newBuckNode;
					bucket_node_write(m2mem, m2alloc, buffer, ptr_prevNode, prevNode);
				}
			}else{
				ptr_t ptr_newBuckNode = bucket_node_create(m2mem, m2alloc, buffer, currNode.ptr_next, entry);
				currNode.ptr_next = ptr_newBuckNode;
				bucket_node_write(m2mem, m2alloc, buffer, ptr_currNode, currNode);
			}
		}
	}
	return ptr_hdList;
}
// vals operations // unsorted list
ptr_t vals_node_add(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr, data_t val){
	ptr_t ptr_hdList = custom_malloc(m2mem, m2alloc, buffer, sizeof(struct vals_node_t));
	struct vals_node_t node;
	node.ptr_next = ptr;
	node.val = val;
	vals_node_write(m2mem, m2alloc, buffer, ptr_hdList, node);
	return ptr_hdList;
}