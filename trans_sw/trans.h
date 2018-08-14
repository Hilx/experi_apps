#include "malloc_wrapper.h"

#include <stdio.h> // printf() - sw only

#define NULL_PTR 0xFFFF0007
#define HASHTABLE_NUM_BUCKETS 32

#define LIST_OFFSET_PTR_NEXT 0
#define LIST_OFFSET_KEY      1
#define LIST_OFFSET_VAL      2
#define BUCK_OFFSET_PTR_NEXT 0
#define BUCK_OFFSET_PTR_VALS 1
#define BUCK_OFFSET_KEY		 2
#define VALS_OFFSET_PTR_NEXT 0
#define VALS_OFFSET_VAL      1

struct pair_t{
	data_t key;
	data_t val;
};
struct list_node_removed_t{
	ptr_t ptr_hdList;
	struct pair_t pair;
};
struct list_node_t{
	ptr_t ptr_next;
	struct pair_t pair;
};
struct bucket_node_t{
	ptr_t ptr_next;
	ptr_t ptr_vals;
	data_t key;
};
struct vals_node_t{
	ptr_t ptr_next;
	data_t val;
};

// mem-acc low funcs
// mem reads
struct list_node_t list_node_read(ptr_t ptr);
ptr_t hashtable_bucket_get(ptr_t ptr, data_t key);
struct bucket_node_t bucket_node_read(ptr_t ptr);
struct vals_node_t vals_node_read(ptr_t ptr);
// mem writes
void list_node_write(ptr_t ptr, struct list_node_t node);
void hashtable_bucket_update(ptr_t ptr, data_t key, ptr_t ptr_updated);
void bucket_node_write(ptr_t ptr, struct bucket_node_t node);
void vals_node_write(ptr_t ptr, struct vals_node_t node);

// mid low funcs
// in-trann. list operations // unsorted list
ptr_t list_node_add(ptr_t ptr, struct pair_t entry);
struct list_node_removed_t list_node_remove(ptr_t ptr);
void list_destroy(ptr_t ptr);
// hashtable oprations
data_t hashing(data_t key);
ptr_t hashtable_create(void);
void hashtable_entry_add(ptr_t ptr, struct pair_t entry);
// bucket operations // sorted list
ptr_t bucket_node_create(ptr_t ptr_next, struct pair_t entry);
ptr_t bucket_node_add(ptr_t ptr, struct pair_t entry);
// vals operations // unsorted list
ptr_t vals_node_add(ptr_t ptr, data_t val);

// high level oprations
#define LABEL_BEGIN 0xEEEE0000
#define LABEL_END   0xEEEE1111

struct process_pack_t{
	ptr_t ptr_hashtable;
	ptr_t ptr_list;
	data_t key;
	data_t key_valid;
};

struct process_pack_t string_process(struct process_pack_t ipack, data_t data);

// print it out
void print_table(ptr_t ptr);