#include "mm.h"

mem_block* free_list = NULL;

int init_mm(struct limine_memmap_entry** entries, size_t entry_count) {

  // Initialize free list
  for (size_t i = 0; i < entry_count; i++) {
    struct limine_memmap_entry* entry = entries[i];
    if (entry->type == LIMINE_MEMMAP_USABLE) {
      mem_block* block = (mem_block*)entry->base;
      block->size = entry->length;
      block->is_free = 1;
      block->next = free_list;
      free_list = block; 
    }
  }
  return 0;
}

void* malloc(size_t size) {
  
  // Align size
  size_t total_size = size + sizeof(mem_block);
  total_size = (total_size + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
  
  mem_block* prev = NULL;
  mem_block* curr = free_list;
  
  while (curr) {
    if (curr->is_free && curr->size >= total_size) {
      if (curr->size >= total_size + sizeof(mem_block)) {
        // Split block
        mem_block* new_block = (mem_block*)((uint8_t*)curr + total_size);
        new_block->size = curr->size - total_size;
        new_block->is_free = 1;
        new_block->next = curr->next;
        curr->next = new_block;
      }
      curr->is_free = 0;
      curr->size = total_size;
      if (prev) prev->next = curr->next;
      else free_list = curr->next;
      return (void*)(curr + 1); 
    }
    prev = curr;
    curr = curr->next;
  }

  return NULL; // Out of memory
}

void free(void* ptr) {
  if (!ptr) return;

  mem_block* block = (mem_block*)ptr - 1;
  block->is_free = 1;

  // Coalesce with next block if free
  mem_block* next = block->next;
  if (next && next->is_free) {
    block->size += next->size;
    block->next = next->next;
  }

  // Add to free list
  block->next = free_list;
  free_list = block;
}