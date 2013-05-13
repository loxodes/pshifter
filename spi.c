#include <msp430.h>
#include <stdint.h>
#include "pins.h"
#include "spi.h"

void init_spi(void)
{
    SPISEL |= SCK | MOSI | MISO;

    UCB0CTL1 |= (UCSWRST); // disable SPI while altering settings
    
    // (SMCLK / (UCB0BR0 + UCB0BR1 * 256)) is SPI clock
    UCB0BR0 = 0;
    UCB0BR1 = 1; // 62.5 kHz for 16 MHz SMCLK

    UCB0CTL1 |= UCSSEL_2; // SMCLK clock select, master mode (p457)

    UCB0CTL0 = UCMST + UCSYNC + UCCKPH; // SPI master, synchronous mode, LSB first, 3 pin SPI, rising edge  (p457)

    UCB0CTL1 &= ~(UCSWRST); // disable software reset enable

}

void spi_msb(uint8_t e)
{
    UCB0CTL1 |= (UCSWRST); // disable SPI while altering settings
    if(e) {
        UCB0CTL0 |= UCMSB;
    }
    else {
        UCB0CTL0 &= ~(UCMSB);
    }

    UCB0CTL1 &= ~(UCSWRST); // disable software reset enable
}


// exchange byte over SPI (busy waiting)
uint8_t spi_exchange(uint8_t b)
{
    UCB0TXBUF = b;
    while(UCB0STAT & UCBUSY);
    b = UCB0RXBUF;
    return b;
}

uint32_t spi_exchange32(uint32_t w)
{
    return 0;
}
