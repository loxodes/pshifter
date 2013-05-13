#include <msp430.h>
#include <stdint.h>
#include "pins.h"

void init_switch(void)
{
    RFSWITCHDIR |= SELPATH | INVPATH;
    RFSWITCHOUT |= SELPATH;
    RFSWITCHOUT &= ~(INVPATH);
}


void switch_set(uint8_t path)
{
    RFSWITCHOUT &= ~(SELPATH);

    if(path == RXPATH || path == TXPATH) {
        RFSWITCHOUT |= path;
    }
}
