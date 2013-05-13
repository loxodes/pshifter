#include <msp430.h>
#include <stdint.h>
#include "pins.h"
#include "uart.h"
#include "intmacro.h"
#include "delay.h"

uint8_t rx_buff[RXBUFFSIZE];
uint8_t tx_buff[TXBUFFSIZE];

volatile uint8_t rxiin;
volatile uint8_t rxiout;
volatile uint8_t txiin; 
volatile uint8_t txiout;
volatile uint8_t rxempty;
volatile uint8_t txempty;

void init_uart(void)
{
    UCA0CTL1 = UCSWRST; // set UCSWRST (recommended before initializing USCI)
    UARTSEL |= RXD | TXD; // set UART pins to UART mode

    UCA0CTL1 |= UCSSEL1; // SMCLK clock select
    UCA0CTL0 = 0; // 8 bit, 1 stop, no parity, async

    // set baud rate SMCLK / (UCA0BR0 + 256 * UCA0BR1)
    UCA0BR1 = 6;
    UCA0BR0 = 130; // 9600 baud, no oversampling

    UCA0MCTL = 0;// UCBRS_3;
    UCA0CTL1 &= ~(UCSWRST); //clear reset

    UC0IE |= UCA0TXIE | UCA0RXIE; // enable UART interrupts

    rxiin = 0;
    rxiout = 0;

    txiin = 0;
    txiout = 0;

    rxempty = 1;
    txempty = 1;
}

uint8_t uart_put(uint8_t c) {
    uint8_t t;
    uint8_t res = BUFF_NOMINAL;

    dint();

    if(txempty) {
        UCA0TXBUF = c;
        txempty = 0;
    }

    else {
        t = (txiin + 1) % TXBUFFSIZE;
        if(t != txiout) {
            tx_buff[txiin] = c;
            txiin = t;
        }
        else {
            res = BUFF_FULL;
        }

    }

    eint();

    return res;
}

uint16_t uart_tx_empty(void)
{
    return txempty;
}

uint16_t uart_rx_vol(void) {
    if(rxiout > rxiin) {
        return rxiin - rxiout + RXBUFFSIZE;
    }
    return rxiin - rxiout;
}

uint8_t uart_grab(void) {
    uint8_t c;

    if(rxempty) {
        return 0;
    }

    else {
        dint();
        c = rx_buff[rxiout];
        rxiout = (rxiout + 1) % RXBUFFSIZE;
        if(rxiout == rxiin) {
            rxempty = 1;
        }
        eint();
    }
    return c;
}

uint16_t uart_grab16(void)
{
    return uart_grab() | (uart_grab() << 8);
}

void uart_put16(uint16_t w)
{
    uart_put(w & 0xFF);
    uart_put((w >> 8) & 0xFF);
}

interrupt(USCIAB0TX_VECTOR) TX_ISR(void)
{
    uint8_t flags = UC0IFG & UC0IE;

    if(flags & UCA0TXIFG) {
        if(txiin != txiout) {
            delay_ms(3);
            UCA0TXBUF = tx_buff[txiout];
            txiout = (txiout + 1) % TXBUFFSIZE;
        }
        else { 
            txempty = 1;
            UC0IFG &= ~UCA0TXIFG;
        }
    }

}

interrupt(USCIAB0RX_VECTOR) RX_ISR(void)
{
    uint8_t flags = UC0IFG & UC0IE;
    uint16_t t;

    if(flags & UCA0RXIFG) {
        rxempty = 0;
        t = (rxiin + 1) % RXBUFFSIZE;

        if(t != rxiout) {
            rx_buff[rxiin] = UCA0RXBUF;
            rxiin = t;
        }
        else {
            t = UCA0RXBUF; // read byte to clear interrupt flag
        }
    }
}
