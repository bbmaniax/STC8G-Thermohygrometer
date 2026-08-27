// View

#ifdef __INTELLISENSE__
#define __SDCC_SYNTAX_FIX
#endif

#include "view.h"
#include "model.h"

void View_init(void) {
    SSD1306_Display_init();
}

void View_begin(void) {
    SSD1306_Display_begin();
    // TODO
}

void View_update(void) {
    // TODO
}
