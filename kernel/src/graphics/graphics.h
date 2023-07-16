#include "limine/limine.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct
{
    uint8_t a;
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Color;

typedef struct
{
    uint32_t *pixels;
    uint32_t width;
    uint32_t height;
} Image;

typedef struct
{
    uint8_t *pixels;
    int charwidth;
    int charheight;
} Font;

typedef struct
{
    int x;
    int y;
} Position;

typedef struct
{
    uint8_t magic[2];
    uint8_t mode;
    uint8_t height;
} FontHeader;

bool graphics_available();
int init_video(struct limine_framebuffer *framebuffer);
void swap_buffers(void);
void draw_pixel(int x, int y, uint32_t color);
void draw_rectangle(int x, int y, int width, int height, uint32_t color);
uint32_t get_pixel(int x, int y);
void clear_screen(uint32_t color);
Position draw_string(char *string, int x, int y, Font f, uint32_t col);
void draw_char(char c, int x, int y, Font f, uint32_t col);