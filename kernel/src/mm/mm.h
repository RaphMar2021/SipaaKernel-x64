#include <stdint.h>
#include <stddef.h>
#include <limine/limine.h>

#define PAGE_SIZE 4096
#define MAX_MEMORY (128ULL * 1024 * 1024 * 1024)
#define PAGE_COUNT (MAX_MEMORY / PAGE_SIZE)
#define PAGE_MASK 0xFFFFFFFFFFFFF000

typedef struct mem_block
{
    struct mem_block *next;
    size_t size;
    int is_free;
} mem_block;

int init_mm(struct limine_memmap_entry **entries, uint64_t entry_count);
void *malloc(size_t size);
void free(void *ptr);