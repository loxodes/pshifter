#include <msp430.h>
#include <stdint.h>
#include "commands.h"
#include "delay.h"
#include "pins.h"
#include "rs485.h"
#include "uart.h"

void init_rs485(void)
{
    RS485DIR |= RE | DE;
    rs485_mode(RS485_STANDBY);
}

uint8_t rs485_mode(uint8_t m)
{
    switch(m) {
        case RS485_READ:
            RS485OUT &= ~(DE + RE);
            break;
        case RS485_SEND:
            RS485OUT |= DE + RE;
            break;
        case RS485_STANDBY:
        default:
            RS485OUT |= RE;
            RS485OUT &= ~(DE); 
            break;
    }

    return 0;
}

uint16_t rs485_send_message(message *msg)
{
    rs485_mode(RS485_SEND);
    uart_put(ENQ);
    uart_put16(msg->sourceaddr);
    uart_put(RS);
    uart_put16(msg->destaddr);
    uart_put(STX);

    if(msg->valid) {
        uart_put(ACK);
        uart_put(msg->cmd);
        uart_put(msg->re_len);
        if(msg->re_len) {
            uart_put(msg->response);
        }
    }

    else {
        uart_put(NAK);
        uart_put(msg->cmd);
    }

    uart_put(ETX);

    while(!uart_tx_empty()); // wait for tx buffer to empty..
    delay_ms(5); // wait for last byte to be sent..

    rs485_mode(RS485_READ);

    return 0; // TODO: eventually indicate sucessful send?
}

void rs485_get_message(message *msg)
{
    rs485_mode(RS485_READ);
    msg->cmd = INVALID;
    msg->valid = INVALID;
    msg->dir = INCOMING;
    msg->destaddr = INVALID;

    if (uart_rx_vol() == 0) {
        return;
    }

    if(uart_grab() != ENQ) {
        return;
    }
    
    while(uart_rx_vol() < 7);

    msg->destaddr = uart_grab16();

    if(uart_grab() != RS) {
        return;
    }

    msg->sourceaddr = uart_grab16();

    if(uart_grab() != STX) {
        return;
    }

    msg->dir = uart_grab();

    switch(msg->dir) {
        case SOH:
        case ACK:
            while(uart_rx_vol() < 2);
            msg->cmd = uart_grab();
            msg->len = uart_grab();
           
            // check if there is enough space to store the payload
            // prevents locking if incorrect value for payload is receieved
            // should use: if(RXBUFFSIZE - uart_rx_vol() < msg->len + 1) {
            if(msg->len + 1 > MAXPAYLOAD) {
                return;
            }

            while(uart_rx_vol() < msg->len + 1);

            if(msg->len) { // TODO: currently assumes 16 bit payloads, this is bad
                msg->payload = uart_grab16();
            }

            break;
        case NAK:
            while(uart_rx_vol() < 2);
            msg->cmd = uart_grab();
            break;
        default:
            return;
    }

    if(uart_grab() != ETX) {
        return;
    }

    msg->valid = VALID;
}

