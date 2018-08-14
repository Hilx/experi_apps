#include "trans.h"

// in-trann. list operations // unsorted list
ptr_t list_node_add(ptr_t ptr, struct pair_t entry){
	printf("- list node add \n");
	ptr_t ptr_hdList = custom_malloc(sizeof(struct list_node_t));
	struct list_node_t node;
	node.ptr_next = ptr;
	node.pair.key = entry.key;
	node.pair.val = entry.val;
	list_node_write(ptr_hdList, node);
	return ptr_hdList;
}
struct list_node_removed_t list_node_remove(ptr_t ptr){
	printf("- list node remove \n");
	struct list_node_removed_t returnSet;
	returnSet.ptr_hdList = ptr;
	if(ptr != NULL_PTR){
		struct list_node_t node;
		node = list_node_read(ptr);
		returnSet.pair = node.pair;
		returnSet.ptr_hdList = node.ptr_next;
		custom_free(ptr);
	}
	return returnSet;
}
void list_destroy(ptr_t ptr){
	ptr_t ptr_hdList = ptr;
	while(ptr_hdList != NULL_PTR){
		struct list_node_t node;
		node = list_node_read(ptr_hdList);
		custom_free(ptr_hdList);
		ptr_hdList = node.ptr_next;
	}
}
// hashtable oprations
data_t hashing(data_t key){
	return key % HASHTABLE_NUM_BUCKETS;
}
ptr_t hashtable_create(void){
	ptr_t ptr = custom_malloc(sizeof(data_t) * HASHTABLE_NUM_BUCKETS);
	for(int i = 0; i < HASHTABLE_NUM_BUCKETS; i++){
		hashtable_bucket_update(ptr, i, NULL_PTR);
	}
	return ptr;
}
void hashtable_entry_add(ptr_t ptr, struct pair_t entry){
	printf("- hash entry add \n");
	ptr_t ptr_buck = hashtable_bucket_get(ptr, entry.key);
	ptr_t ptr_new = bucket_node_add(ptr_buck, entry);
	if(ptr_buck != ptr_new){
		hashtable_bucket_update(ptr, entry.key, ptr_new);
	}
}
// bucket operations // sorted list
ptr_t bucket_node_create(ptr_t ptr_next, struct pair_t entry){
	ptr_t ptr_newValsNode = vals_node_add(NULL_PTR, entry.val);
	ptr_t ptr_newBuckNode = custom_malloc(sizeof(struct bucket_node_t));
	struct bucket_node_t node;
	node.ptr_next = ptr_next;
	node.ptr_vals = ptr_newValsNode;
	node.key = entry.key;	
	bucket_node_write(ptr_newBuckNode,node);
	return ptr_newBuckNode;
}
ptr_t bucket_node_add(ptr_t ptr, struct pair_t entry){
	ptr_t ptr_hdList = ptr;
	if(ptr == NULL_PTR){
		ptr_t ptr_newBuckNode = bucket_node_create(ptr, entry);
		ptr_hdList = ptr_newBuckNode;
	}else{ // have to search for a good spot for new node or existence check, if exists only add the val
		struct bucket_node_t currNode = bucket_node_read(ptr);
		struct bucket_node_t prevNode;
		ptr_t ptr_currNode = ptr;
		ptr_t ptr_prevNode = NULL_PTR;
		while(currNode.key < entry.key && currNode.ptr_next != NULL_PTR){
			ptr_prevNode = ptr_currNode;
			prevNode = currNode;
			ptr_currNode = currNode.ptr_next;
			currNode = bucket_node_read(currNode.ptr_next);
		}
		if(currNode.key == entry.key){ // bucket node already exists, don't create new bucket node
			ptr_t ptr_vals = vals_node_add(currNode.ptr_vals, entry.val);
			currNode.ptr_vals = ptr_vals;
			bucket_node_write(ptr, currNode);
		}else{ // bucket node not found, create new bucket node
			if(entry.key < currNode.key){
				ptr_t ptr_newBuckNode = bucket_node_create(ptr_currNode, entry);
				if(ptr_prevNode == NULL_PTR){
					ptr_hdList = ptr_newBuckNode;
				}else{
					prevNode.ptr_next = ptr_newBuckNode;
					bucket_node_write(ptr_prevNode, prevNode);
				}
			}else{
				ptr_t ptr_newBuckNode = bucket_node_create(currNode.ptr_next, entry);
				currNode.ptr_next = ptr_newBuckNode;
				bucket_node_write(ptr_currNode, currNode);
			}
		}
	}
	return ptr_hdList;
}
// vals operations // unsorted list
ptr_t vals_node_add(ptr_t ptr, data_t val){
	ptr_t ptr_hdList = custom_malloc(sizeof(struct vals_node_t));
	struct vals_node_t node;
	node.ptr_next = ptr;
	node.val = val;
	vals_node_write(ptr_hdList, node);
	return ptr_hdList;
}