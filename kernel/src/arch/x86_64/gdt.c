#include "gdt.h"
#include <device/serial/serial.h>
#include <lib/liblogging/liblogging.h>
#include <lib/libconsole/libconsole.h>

gdt_entry gdt[0xFF];
gdt_pointer gdtr;
tss_entry_t tss_entry;

extern void load_gdt(gdt_pointer*);

void set_gdt_entry(int i, uint16_t limit, uint16_t base, uint8_t access, uint8_t flags) {
    gdt[i].base0 = base & 0xFFFF;
    gdt[i].base1 = (base >> 16) & 0xFF;
    gdt[i].base2 = (base >> 24) & 0xFF;
    gdt[i].limit0 = limit & 0xFFFF;
    gdt[i].access = access;
    gdt[i].limit1_flags = ((limit >> 16) & 0x0F) | (flags & 0xF0);
}
void set_tss_gate(int i, uint64_t base, uint32_t limit)
{
    gdt[i].base0 = base & 0xFFFF;
    gdt[i].base1 = (base >> 16) & 0xFF;
    gdt[i].base2 = (base >> 24) & 0xFF;
    gdt[i].limit0 = limit & 0xFFFF;
    gdt[i].access = 0xE9;
    gdt[i].limit1_flags = ((limit >> 16) & 0x0F) | 0x00;
}

void init_tss(uint64_t rsp0)
{
    char *ptr_c = (char *)&tss_entry;
    for (int i = 0; i < sizeof(tss_entry_t); i++)
    {
        ptr_c[i] = (char)0;
    }
    tss_entry.rsp0 = rsp0;
}

int init_gdt(uint64_t rsp0) {
    log(LOG_DEBUG, "Setting up GDT entries...");
    set_gdt_entry(0, 0, 0, 0, 0);
    set_gdt_entry(1, 0xFFFFF, 0, 0x9A, 0xAF);
    set_gdt_entry(2, 0xFFFFF, 0, 0x92, 0xAF);
    set_gdt_entry(3, 0xFFFFF, 0, 0x96, 0xAF);
    set_gdt_entry(4, 0xFFFFF, 0, 0xFA, 0xAF);
    set_gdt_entry(5, 0xFFFFF, 0, 0xF2, 0xAF);
    set_gdt_entry(6, 0xFFFFF, 0, 0xF6, 0xAF);
    set_tss_gate(7, (uint64_t)&tss_entry, sizeof(tss_entry_t));

    serial_puts(" OK\n");
    printf(" OK\n");

    log(LOG_DEBUG, "Setting up GDT pointer...");
    gdtr.size = sizeof(gdt) - 1;
    gdtr.offset = (uint64_t)&gdt;
    serial_puts(" OK\n");
    printf(" OK\n");

    log(LOG_DEBUG, "init_gdt() Loading GDT/TSS into your computer...");
    init_tss(rsp0);
    load_gdt(&gdtr);
    load_tss();
    serial_puts(" OK\n");
    printf(" OK\n");

    return 0;
}

void set_kernel_stack(uint64_t stack)
{
    tss_entry.rsp0 = stack;
}