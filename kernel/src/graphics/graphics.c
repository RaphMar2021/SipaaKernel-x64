#include "graphics.h"
#include <stdbool.h>

struct limine_framebuffer *current_framebuffer;

void *back_buffer;
void *front_buffer;

uint32_t term_color = 0xffffff;
int term_x, term_y;
bool gavailable = false;

int init_video(struct limine_framebuffer *framebuffer)
{
    current_framebuffer = framebuffer;
    front_buffer = framebuffer->address;
    back_buffer = (void *)((uint64_t)framebuffer->address + framebuffer->pitch * framebuffer->height);
    gavailable = true;

    return 0;
}

bool graphics_available()
{
    return gavailable;
}

void swap_buffers(void)
{
    void *temp = front_buffer;
    front_buffer = back_buffer;
    back_buffer = temp;

    for (uint64_t i = 0; i < current_framebuffer->pitch * current_framebuffer->height; i++)
    {
        ((uint8_t *)current_framebuffer->address)[i] = ((uint8_t *)front_buffer)[i];
    }
}

void draw_pixel(int x, int y, uint32_t color)
{
    uint32_t *pixel_address = (uint32_t *)((uint64_t)back_buffer + y * current_framebuffer->pitch + x * (current_framebuffer->bpp / 8));

    *pixel_address = color;
}

void draw_rectangle(int x, int y, int width, int height, uint32_t color)
{
    for (int i = x; i < x + width; i++)
    {
        for (int j = y; j < y + height; j++)
        {
            draw_pixel(i, j, color);
        }
    }
}

uint32_t get_pixel(int x, int y)
{
    uint32_t *pixel_address = (uint32_t *)((uint64_t)back_buffer + y * current_framebuffer->pitch + x * (current_framebuffer->bpp / 8));

    return *pixel_address;
}

void clear_screen(uint32_t color)
{
    for (int y = 0; y < current_framebuffer->height; y++)
    {
        for (int x = 0; x < current_framebuffer->width; x++)
        {
            draw_pixel(x, y, color);
        }
    }
}

void draw_char(char c, int x, int y, Font f, uint32_t col)
{
    uint8_t *offset = f.pixels + sizeof(FontHeader) + 16 * c;

    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (offset[i] & (1 << j))
            {
                draw_pixel(x + 8 - j, y + i, col);
            }
        }
    }
}

Position draw_string(char *string, int x, int y, Font f, uint32_t col)
{
    int cursorx = x;
    int cursory = y;

    for (int i = 0; string[i] != '\0'; i++)
    {
        if (string[i] == '\n')
        {
            cursorx = x;
            cursory += f.charheight;
        }
        else
        {
            draw_char(string[i], cursorx, cursory, f, col);
            cursorx += f.charwidth;
        }
    }

    Position p;
    p.x = cursorx;
    p.y = cursory;

    return p;
}