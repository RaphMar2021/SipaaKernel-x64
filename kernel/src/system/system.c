#include "graphics/graphics.h"
#include "resources/resources.h"
#include <stdbool.h>

void _panic(char *message, char *file, char *line, struct interrupt_frame *frame)
{
    if (graphics_available() == true)
    {
        Font f;
        f.charwidth = DEFFONT_CHARWIDTH;
        f.charheight = DEFFONT_CHARHEIGHT;
        f.pixels = &deffont;

        clear_screen(0xCC0000);
        Position pos1 = draw_string("panic(0) Your computer ran into a big problem.\npanic(0) Message : ", 10, 10, f, 0xFFFFFF);
        Position pos2 = draw_string(message, pos1.x, pos1.y, f, 0xFFFFFF);
        Position pos3 = draw_string("panic(0) File : ", 10, pos2.y + 16, f, 0xFFFFFF);
        Position pos4 = draw_string(file, pos3.x, pos3.y, f, 0xFFFFFF);
        Position pos5 = draw_string("panic(0) Line : ", 10, pos4.y + 16, f, 0xFFFFFF);
        Position pos6 = draw_string(line, pos5.x, pos5.y, f, 0xFFFFFF);
        draw_string("panic(0) System halted!", 10, pos6.y + 16, f, 0xFFFFFF);

        swap_buffers();
    }

    serial_puts("panic(0) Your computer ran into a big problem.\npanic(0) Message : ");
    serial_puts(message);
    serial_puts("\npanic(0) File : ");
    serial_puts(file);
    serial_puts("\npanic(0) Line : ");
    serial_puts(line);
    serial_puts("\npanic(0) System halted!");

    while (1)
    {
        __asm__("hlt");
    }
}