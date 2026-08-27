// main.c

#ifdef __INTELLISENSE__
#define __SDCC_SYNTAX_FIX
#endif

#include "app.h"

void main(void) {
    setup();
    while (1)
    {
        loop();
    }
}
