#include "gdt.h"
#include "devices/serial/serial.h"

gdt_entry gdt[5];
gdt_pointer gdtr;

extern void load_gdt(gdt_pointer*);

void set_gdt_entry(int i, uint16_t limit, uint16_t base, uint8_t access, uint8_t flags) {
    gdt[i].base0 = base & 0xFFFF;
    gdt[i].base1 = (base >> 16) & 0xFF;
    gdt[i].base2 = (base >> 24) & 0xFF;
    
    gdt[i].limit0 = limit & 0xFFFF;
    
    gdt[i].access = access;
    
    gdt[i].limit1_flags = ((limit >> 16) & 0x0F) | (flags & 0xF0);
}

int init_gdt(void) {
    serial_puts("init_gdt() Setting up GDT entries...");
    set_gdt_entry(0, 0, 0, 0, 0);
    set_gdt_entry(1, 0, 0, 0x9A, 0xA0);
    set_gdt_entry(2, 0, 0, 0x92, 0xA0);
    set_gdt_entry(3, 0, 0, 0xFA, 0xA0);
    set_gdt_entry(4, 0, 0, 0xF2, 0xA0);
    serial_puts(" OK\n");

    serial_puts("init_gdt() Setting up GDT pointer...");
    gdtr.size = sizeof(gdt) - 1;
    gdtr.offset = (uint64_t)&gdt;
    serial_puts(" OK\n");
    
    serial_puts("init_gdt() Loading GDT into your computer...");
    load_gdt(&gdtr);
    serial_puts(" OK (if in QEMU, open compatmonitor and type 'info registers' and see if GDT value isn't null)\n");

    return 0;
}