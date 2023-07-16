#include "stdlibrary.h"

char* strcat(char* dest, const char* src) {
    char *walk = dest;
    while (*walk) walk++;
    while (*src) *walk++ = *src++;
    *walk = 0;
    return dest;
}