typedef long int sw_word_t;
typedef int hls_word_t;
//modifye when modifying to hls
typedef hls_word_t data_t;
typedef hls_word_t ptr_t;

// ZEPALLOC
#define BASE 0x43c00000
#define INIT 0x43c00100
#define RESE 0x43c00080
#define BASE_SIZE 16
#define MAX_ALLOC_INDEX 20

// SYSALLOC
#define MMU_BASE 0x43C00000
#define MMU_TOKEN 0
#define MMU_STATUS 4
#define MMU_RESULT 8
#define MMU_CMD 12
#define MMU_FREE 16

// functions
ptr_t custom_malloc(data_t * m2mem, data_t * m2alloc, data_t size);
volatile void custom_free(data_t * m2mem, data_t * m2alloc, ptr_t ptr);
volatile void custom_alloc_init(data_t * m2mem, data_t * m2alloc);