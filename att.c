// jon klein, kleinjt@ieee.org
// driver for SKY12343-364LF digital attenuator, serial mode

#include <msp430.h>
#include <stdint.h>
#include "pins.h"
#include "spi.h"
#include "delay.h"
#include "att.h"
#include "pshifter_config.h"

uint8_t att_level;
const uint8_t preg2attreg[64] = ATT_CALTABLE;

void init_att()
{
    ATTDIR |= ATT_LE;
    ATTOUT &= ~(ATT_LE);
    att_clear();
}

void att_clear()
{
    att_set(127);
}

void att_set(uint8_t level)
{
    att_level = level;
    att_set_raw(level);
}

void att_set_raw(uint8_t level)
{
    uint16_t w = (((0) << ATT_ADDR_BASE) | (level & 0xFF));
    att_send(w);
    att_send(w);
}

void att_flatten(uint8_t phase)
{
    att_set_raw(preg2attreg[phase] + att_level);
}
uint8_t att_get(void)
{
    return att_level;
}

void att_send(uint16_t w)
{
    spi_msb(LAST);
    ATTOUT &= ~(ATT_LE);
    delay_us(1);
    spi_exchange(w & 0xFF);
    spi_exchange((w >> 8) & 0xFF);
    delay_us(40);
    ATTOUT |= ATT_LE;
    delay_us(20);
    ATTOUT &= ~(ATT_LE);
}
