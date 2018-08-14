#include <stdlib.h> // function for malloc()

typedef long int sw_word_t;
typedef int hls_word_t;
//modifye when modifying to hls
typedef sw_word_t data_t;
typedef sw_word_t ptr_t;

ptr_t custom_malloc(data_t size);
void custom_free(ptr_t ptr);

