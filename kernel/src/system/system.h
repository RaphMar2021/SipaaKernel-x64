#include <stdint.h>

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define panic(message, frame) _panic(message, __FILE__, TOSTRING(__LINE__), frame)