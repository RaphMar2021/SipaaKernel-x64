#include <stdint.h>
#include <stdbool.h>
#include "pic.h"

struct idt_entry {
  uint16_t offset_low;
  uint16_t selector; 
  uint8_t ist;      
  uint8_t type_attr;
  uint16_t offset_middle;
  uint32_t offset_high;
  uint32_t zero; 
} __attribute__((packed));

struct idt_ptr {
  uint16_t limit;
  uint64_t base; 
} __attribute__((packed));

int init_idt();
void idt_set_entry(int num, void* handler, uint8_t ist);