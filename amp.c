#include <msp430.h>
#include <stdint.h>
#include "pins.h"
#include "adc.h"
#include "amp.h"

// init and disable all amplifiers
void init_amps(void)
{
    AMPENOUT &= ~(PAMP_EN);
    AMPENDIR |= PAMP_EN;
    
    AMPDETAE |= AMP_DET;

    LNAENOUT &= ~(LNA_EN);
    LNAENDIR |= LNA_EN;
}

void pamp_set(uint8_t s)
{
    if(s == AMP_ENABLE) {
        AMPENOUT |= PAMP_EN;
    }
    else {
        AMPENOUT &= ~(PAMP_EN);
    }
}

void lna_set(uint8_t s)
{
    if(s == AMP_ENABLE) {
        LNAENOUT |= LNA_EN;
    }
    else {
        LNAENOUT &= ~(LNA_EN);
    }
}

// reads the power of the power amplifier, result in fractional dBm
// piecewise linear approximation of SST12LP15B power detect with 2.5V vref
uint16_t pamp_readpower(void)
{
    uint16_t power;

    uint16_t detv = adc_read(AMP_DET_ADCCHAN);

    if(detv > 245)
    {
        power = 150 + (((detv-245) * 7))/10;
    }
    else if (detv > 512)
    {
        power = 80 + (((detv-205) * 2))/10;
    }
    else
    {
        power = (detv - 195);
    }

    return power;
}

