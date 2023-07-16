#include "limine/limine.h"
#include "architecture/x86_64/gdt.h"
#include "architecture/x86_64/idt.h"
#include "devices/serial/serial.h"
#include "devices/ps2/mouse.h"
#include "graphics/graphics.h"
#include "mm/mm.h"
#include "resources/resources.h"
#include "stdlibrary/stdlibrary.h"
#include "system/system.h"

#include <stdint.h>

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0};

static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0};

static volatile struct limine_bootloader_info_request info_request = {
    .id = LIMINE_BOOTLOADER_INFO_REQUEST,
    .revision = 0};

void print_result(int result)
{
    if (result == 0)
    {
        serial_puts("_start() Success!\n");
    }
    else
    {
        serial_puts("_start() It failed...\n"); //
    }
}

void print_bootloaderinfo()
{
    #if limine
    serial_puts("_start() Limine bootloader is used!\n");
    serial_puts("_start() Limine ");
    serial_puts(info_request.response->version);
    serial_puts(" is used!\n");
    return;
    #endif
    panic("No bootloader declaration found", NULL);
}


void _start(void)
{
    int r = 0;
    print_bootloaderinfo();
    serial_puts("_start() Entry point reached!\n");
    serial_puts("_start() Getting Limine's framebuffer...\n");
    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];
    struct limine_memmap_entry *memmap_entry = memmap_request.response->entries;

    init_serial();
    init_mm(memmap_entry, memmap_request.response->entry_count);

    void *mem_zone = malloc(700);

    if (mem_zone == (void*)0)
    {
        serial_puts("_start() malloc failed\n");
    }
    else
    {
        free(mem_zone);
    }

    serial_puts("_start() Initializing video...\n");
    r = init_video(framebuffer);
    print_result(r);
    serial_puts("_start() Initializing Global Descriptor Table...\n");
    r = init_gdt();
    print_result(r);
    serial_puts("_start() Initializing Interrupt Descriptor Table...\n");
    r = init_idt();
    print_result(r);
    serial_puts("_start() Initializing PS2 mouse...\n");
    r = init_mouse(800, 600);
    print_result(r);

    int x = 0;
    int y = 0;
    int ttb = 0;
    int ltr = 0;

    Font f;
    f.charwidth = DEFFONT_CHARWIDTH;
    f.charheight = DEFFONT_CHARHEIGHT;
    f.pixels = &deffont;

    serial_puts("_start() SipaaOS has been initialized! Entering kernel's main loop...\n");

    char *test = "Test";

    while (1)
    {
        clear_screen(0x0);
        if (x == 700)
        {
            ltr = 0;
        }
        if (x == 0)
        {
            ltr = 1;
        }

        if (y == 500)
        {
            ttb = 0;
        }
        if (y == 0)
        {
            ttb = 1;
        }

        if (ltr == 1)
        {
            x++;
        }
        else
        {
            x--;
        }

        if (ttb == 1)
        {
            y++;
        }
        else
        {
            y--;
        }
        
        draw_pixel(mouse_getx(), mouse_gety(), 0xFFFFFF);
        draw_string(test, 100, 100, f, 0xFFFFFF);
        draw_rectangle(x,y,100,100,0xFFFFFF);
        swap_buffers();
    }
}
