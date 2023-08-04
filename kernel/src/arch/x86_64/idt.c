// #include <framebuffer/font.h>
#include "idt.h"
#include <device/serial/serial.h>
#include <lib/liblogging/liblogging.h>
#include <lib/libconsole/libconsole.h>
#include <stdint.h>
#include <stddef.h>

struct idt_entry idt[256]; 
struct idt_ptr idtp;

void div_zero_handler() {
  // Handle divide by 0 exception
  //panic("DIV0", NULL);
}

void debug_handler() {
  // Handle debug exception  
  //panic("Debug", NULL);
}

void gpf_handler() {
    // Handle debug exception  
    //panic("GPF", NULL);
}
// ... Other exception handlers

void irq0_handler() {
    // Handle IRQ0  
}

void irq1_handler() {
    log(LOG_DEBUG, "Keyboard IRQ has been raised!\n");
    pic_end_master();

} 

void idt_load() {
    remap_pic();
    outb(PIC1_DATA, 0b11111101);
    outb(PIC2_DATA, 0b11111111);

    // Load to CPU
    log(LOG_DEBUG, "Loading IDT into CPU...");
    asm volatile("lidt %0" : : "m"(idtp)); 
    serial_puts(" OK\n");
    printf(" OK\n");
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
    log(LOG_DEBUG, "Setting up IDT pointer...");
    idtp.limit = sizeof(idt) - 1;
    idtp.base = (uint64_t)&idt;
    serial_puts(" OK\n");
    printf(" OK\n");

    // Exception handlers  
    log(LOG_DEBUG, "Setting up CPU exceptions entries...");
    idt_set_entry(0, div_zero_handler, 0);
    idt_set_entry(1, debug_handler, 0);
    idt_set_entry(13, gpf_handler, 0);
    serial_puts(" OK\n");
    printf(" OK\n");

    // ... Set other exception entries

    // IRQ handlers
    log(LOG_DEBUG, "Setting up IRQ exceptions entries...");
    idt_set_entry(0x20, irq0_handler, 0);
    idt_set_entry(0x21, irq1_handler, 0);
    serial_puts(" OK\n");
    printf(" OK\n");

    idt_load();
    asm("sti");
    return 0;
}