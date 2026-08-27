// Model

#ifdef __INTELLISENSE__
#define __SDCC_SYNTAX_FIX
#endif

#include "model.h"

typedef struct {
    int16_t temperature;
} Model;

static __XDATA Model model;

void Model_init(void) {
    model.temperature = 0;
}

void Model_begin(void) {
    // TODO
}

void Model_update(void) {
    // TODO
}
