#include "graph.h"
// root operations
struct root_t root_read(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr_root){
    struct root_t graph_root;
    graph_root.key_cnt = m2mem[ptr_root/4 + ROOT_OFFSET_KEY_CNT];
    graph_root.ptr_hashtable = m2mem[ptr_root/4 + ROOT_OFFSET_PTR_HASHTABLE];
    return graph_root;
}
volatile void root_write(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr_root, struct root_t graph_root){
    m2mem[ptr_root/4 + ROOT_OFFSET_KEY_CNT] = graph_root.key_cnt;
	m2mem[ptr_root/4 + ROOT_OFFSET_PTR_HASHTABLE] = graph_root.ptr_hashtable;
}
// hash table operations
ptr_t hashtable_create(data_t * m2mem, data_t * m2alloc, data_t * buffer){
    // allocate memory for hash table all entries
    ptr_t ptr_hashtable = custom_malloc(m2mem, m2alloc, 4 * HASHTABLE_NUM_BUCKETS);
    // initialise all ptrs to buckets to NULL
    int i;
    for(i = 0; i < HASHTABLE_NUM_BUCKETS; i++){
        //buffer[i] = NULL_PTR;
    	m2mem[ptr_hashtable/4 + i] = NULL_PTR;
    }
    //memcpy((data_t *)(m2mem + ptr_hashtable/4), buffer, sizeof(data_t) * HASHTABLE_NUM_BUCKETS);
    return ptr_hashtable;
}
volatile void hashtable_update(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr_hashtable, data_t hash_id, ptr_t ptr_bucket_node){
    m2mem[ptr_hashtable/4 + hash_id] = ptr_bucket_node;
    //printf("        updating hashtable: hash_id = %d, bucketPtr = %d (hashtablePtr = %d)\n", hash_id, ptr_bucket_node, ptr_hashtable);
}
ptr_t bucket_ptr_get(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr_hashtable, data_t key){
    data_t hash_id = hashing(key);
    ptr_t ptr = m2mem[ptr_hashtable/4 + hash_id];
    //printf("        fetching from hashtable: hash_id = %d, bucketPtr = %d (hashtablePtr = %d)\n", hash_id, ptr, ptr_hashtable);
    return ptr;
}
struct bucket_node_t bucket_node_read(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr_bucket_node){
    struct bucket_node_t bucket_node;
    bucket_node.ptr_next    = m2mem[ptr_bucket_node/4 + BUCKET_NODE_OFFSET_PTR_NEXT];
    bucket_node.ptr_keyset  = m2mem[ptr_bucket_node/4 + BUCKET_NODE_OFFSET_PTR_KEYSET];
    bucket_node.link_cnt    = m2mem[ptr_bucket_node/4 + BUCKET_NODE_OFFSET_LINK_CNT];
    bucket_node.key         = m2mem[ptr_bucket_node/4 + BUCKET_NODE_OFFSET_KEY];
    //printf("read key [%d] read: link cnt = %d \n", bucket_node.key, bucket_node.link_cnt);
    return bucket_node;
}
volatile void bucket_node_write(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr_bucket_node, struct bucket_node_t bucket_node){
    //printf("write key [%d] write: link cnt = %d \n", bucket_node.key, bucket_node.link_cnt);
	m2mem[ptr_bucket_node/4 + BUCKET_NODE_OFFSET_PTR_NEXT]     = bucket_node.ptr_next;
    m2mem[ptr_bucket_node/4 + BUCKET_NODE_OFFSET_PTR_KEYSET]   = bucket_node.ptr_keyset;
    m2mem[ptr_bucket_node/4 + BUCKET_NODE_OFFSET_LINK_CNT]     = bucket_node.link_cnt;
    m2mem[ptr_bucket_node/4 + BUCKET_NODE_OFFSET_KEY]          = bucket_node.key;

}
struct keyset_node_t keyset_node_read(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr_keyset_node){
    struct keyset_node_t keyset_node;
    keyset_node.ptr_next = m2mem[ptr_keyset_node/4 + KEYSET_NODE_OFFSET_PTR_NEXT];
    keyset_node.key = m2mem[ptr_keyset_node/4 + KEYSET_NODE_OFFSET_KEY];
    return keyset_node;
}
volatile void keyset_node_write(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr_keyset_node, struct keyset_node_t keyset_node){
     m2mem[ptr_keyset_node/4 + KEYSET_NODE_OFFSET_PTR_NEXT] = keyset_node.ptr_next;
     m2mem[ptr_keyset_node/4 + KEYSET_NODE_OFFSET_KEY] = keyset_node.key;
}
/*
// root operations
struct root_t root_read(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr_root){
    memcpy(buffer, (data_t *)(m2mem + ptr_root/4),  sizeof(struct root_t));
    struct root_t graph_root;
    graph_root.key_cnt = buffer[ROOT_OFFSET_KEY_CNT];
    graph_root.ptr_hashtable = buffer[ROOT_OFFSET_PTR_HASHTABLE];
    return graph_root;
}
volatile void root_write(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr_root, struct root_t graph_root){
    buffer[ROOT_OFFSET_KEY_CNT] = graph_root.key_cnt;
    buffer[ROOT_OFFSET_PTR_HASHTABLE] = graph_root.ptr_hashtable;
    memcpy((data_t *)(m2mem + ptr_root/4), buffer, sizeof(struct root_t));
}
// hash table operations
ptr_t hashtable_create(data_t * m2mem, data_t * m2alloc, data_t * buffer){
    // allocate memory for hash table all entries
    ptr_t ptr_hashtable = custom_malloc(m2mem, m2alloc, sizeof(struct hashtable_entry_t) * HASHTABLE_NUM_BUCKETS);
    // initialise all ptrs to buckets to NULL
    for(int i = 0; i < HASHTABLE_NUM_BUCKETS; i++){
        //buffer[i] = NULL_PTR;
    	m2mem[ptr_hashtable/4 + i] = NULL_PTR;
    }
    //memcpy((data_t *)(m2mem + ptr_hashtable/4), buffer, sizeof(data_t) * HASHTABLE_NUM_BUCKETS);
    return ptr_hashtable;
}
volatile void hashtable_update(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr_hashtable, data_t hash_id, ptr_t ptr_bucket_node){
    m2mem[ptr_hashtable/4 + hash_id] = ptr_bucket_node;
}
ptr_t bucket_ptr_get(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr_hashtable, data_t key){
    data_t hash_id = hashing(key);
    return m2mem[ptr_hashtable/4 + hash_id];
}
struct bucket_node_t bucket_node_read(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr_bucket_node){
    memcpy(buffer, (data_t *)(m2mem + ptr_bucket_node/4), sizeof(struct bucket_node_t));
    struct bucket_node_t bucket_node;
    bucket_node.ptr_next    = buffer[BUCKET_NODE_OFFSET_PTR_NEXT];
    bucket_node.ptr_keyset  = buffer[BUCKET_NODE_OFFSET_PTR_KEYSET];
    bucket_node.link_cnt    = buffer[BUCKET_NODE_OFFSET_LINK_CNT];
    bucket_node.key         = buffer[BUCKET_NODE_OFFSET_KEY];
    return bucket_node;
}
volatile void bucket_node_write(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr_bucket_node, struct bucket_node_t bucket_node){
    buffer[BUCKET_NODE_OFFSET_PTR_NEXT]     = bucket_node.ptr_next;
    buffer[BUCKET_NODE_OFFSET_PTR_KEYSET]   = bucket_node.ptr_keyset;
    buffer[BUCKET_NODE_OFFSET_LINK_CNT]     = bucket_node.link_cnt;
    buffer[BUCKET_NODE_OFFSET_KEY]          = bucket_node.key;
    memcpy((data_t *)(m2mem + ptr_bucket_node/4), buffer, sizeof(struct bucket_node_t));
}
struct keyset_node_t keyset_node_read(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr_keyset_node){
    memcpy(buffer, (data_t *)(m2mem + ptr_keyset_node/4), sizeof(struct keyset_node_t));
    struct keyset_node_t keyset_node;
    keyset_node.ptr_next = buffer[KEYSET_NODE_OFFSET_PTR_NEXT];
    keyset_node.key = buffer[KEYSET_NODE_OFFSET_KEY];
    return keyset_node;
}
volatile void keyset_node_write(data_t * m2mem, data_t * m2alloc, data_t * buffer, ptr_t ptr_keyset_node, struct keyset_node_t keyset_node){
     memcpy((data_t *)(m2mem + ptr_keyset_node/4), buffer, sizeof(struct keyset_node_t));
     buffer[KEYSET_NODE_OFFSET_PTR_NEXT] = keyset_node.ptr_next;
     buffer[KEYSET_NODE_OFFSET_KEY] = keyset_node.key;
}
*/
