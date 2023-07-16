// #include <framebuffer/font.h>
#include "idt.h"
#include "devices/serial/serial.h"
#include "graphics/graphics.h"
#include "stdlibrary/stdlibrary.h"
#include "resources/resources.h"
#include "system/system.h"
#include <stdint.h>

struct idt_entry idt[256]; 
struct idt_ptr idtp;

void div_zero_handler(); 
void debug_handler();

void irq0_handler();
void irq1_handler();

void idt_load() {
  // Set IDT pointer
  idtp.limit = sizeof(idt) - 1;
  idtp.base = (uint64_t)&idt;

  // Load to CPU
  asm volatile("lidt %0" : : "m"(idtp)); 
}

void idt_set_entry(int num, void* handler, uint8_t ist) {

  struct idt_entry* entry = &idt[num];

  entry->offset_low = (uint64_t)handler & 0xFFFF;
  entry->selector = 0x08; // Kernel CS
  entry->ist = ist;
  entry->type_attr = 0x8E; // Interrupt gate, present
  entry->offset_middle = ((uint64_t)handler >> 16) & 0xFFFF;  
  entry->offset_high = ((uint64_t)handler >> 32) & 0xFFFFFFFF;
  entry->zero = 0;
}

int init_idt() {

  idt_load();

  // Exception handlers  
  idt_set_entry(0, div_zero_handler, 0);
  idt_set_entry(1, debug_handler, 0);

  // ... Set other exception entries

  // IRQ handlers
  idt_set_entry(0x20, irq0_handler, 0);
  idt_set_entry(0x21, irq1_handler, 0);
  
  // ... Set other IRQ entries  
  return 0;
}

void div_zero_handler() {
  // Handle divide by 0 exception
}

void debug_handler() {
  // Handle debug exception  
}

// ... Other exception handlers

void irq0_handler() {
  // Handle IRQ0  
}

void irq1_handler() {
  // Handle IRQ1
} 

// ... Other IRQ handlers