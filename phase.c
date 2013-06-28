// jon klein, jtklein@alaska.edu
// MIT license
// controls the MAPS-010164 phase shifter

#include <msp430.h>
#include <stdint.h>
#include "delay.h"
#include "pins.h"
#include "spi.h"
#include "att.h"
#include "phase.h"
#include "pshifter_config.h"

uint16_t phase;
const uint8_t phase2phasereg[360] = PHASE_CALTABLE;

void init_phase(void)
{
    PSHIFTDIR |= PSHIFT_EN | PSHIFT_SYNC;
    PSHIFTOUT |= PSHIFT_SYNC;
    PSHIFTOUT &= ~(PSHIFT_EN);
    phase_set(0);
}

void pshifter_state(uint16_t s)
{
    if(s == PSHIFTER_ENABLE){
        PSHIFTOUT |= (PSHIFT_EN);
    }
    else {
        PSHIFTOUT &= ~(PSHIFT_EN);
    }
}

void phase_raw(uint16_t p)
{
    spi_msb(FIRST);    
    PSHIFTOUT &= ~PSHIFT_SYNC; 
    spi_exchange(p & MAPS_MASK);
    PSHIFTOUT |= PSHIFT_SYNC;
    delay_us(5);// min 100ns
    PSHIFTOUT &= ~PSHIFT_SYNC; 
}

void phase_set(uint16_t p)
{
//    p = 360 - (p % 360);
//    phase = ((((p + PHASECAL_OFFSET)<<4) + 45)/90) & 0x3F; // approximate 5.6 degree steps with integers, offset center
    phase = phase2phasereg[p % 360];
    phase_raw(phase);
    delay_us(10);
    att_flatten(phase);
}

uint16_t phase_get(void)
{
    return phase;
}
