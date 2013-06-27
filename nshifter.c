// jon klein, jtklein@alaska.edu
// pshifter r2 microcontroller software
#include <msp430.h>
#include <stdint.h>

#include "commands.h"
#include "pins.h"
#include "spi.h"
#include "switch.h"
#include "uart.h"
#include "adc.h"
#include "amp.h"
#include "att.h"
#include "nshifter.h"
#include "delay.h"
#include "intmacro.h"
#include "rs485.h"
#include "phase.h"
#include "pshifter_config.h"

#define PSHIFTER_VER 2

void init_msp(void)
{
    WDTCTL = WDTPW | WDTHOLD;

    delay_ms(10); 

    // setup clock
    BCSCTL3 = LFXT1S_2; // 3 to 16 MHz crystal
    BCSCTL1 = XTS + DIVA_1 + DIVA_0; // LFXT1 HF mode, ACLK / 8
    BCSCTL2 = SELM_3 | SELS; // LFXT1CLK source for MCLK and SMCLK
    delay_ms(10);  // wait for clock to settle
    IFG1 &= ~OFIFG; // clear OSC fault
}

void init_leds(void)
{
    LEDOUT &= ~LED1;
    LEDDIR |= LED1;
}

void init_gpio(void)
{
    GPIODIR |= GP1;
}

void init_pshifter(void)
{
    dint();

    init_msp();

    init_gpio();
    init_leds();

    init_switch();
    init_amps();

    init_uart();
    init_rs485();

    init_spi();
    init_att();
    init_adc();

    set_mode(STANDBY_MODE);

    init_phase();

    eint();

    // set WDT with ACLK, reset counter
    //WDTCTL = WDTSSEL + WDTPW + WDTCNTCL; 
}

void set_mode(uint16_t mode)
{
    switch(mode) {
        case RECEIVE_MODE:
            pamp_set(AMP_DISABLE);
            pshifter_state(PSHIFTER_DISABLE);
            lna_set(AMP_ENABLE);
            switch_set(RXPATH);
            break;
        case TRANSMIT_MODE:
            switch_set(TXPATH);
            lna_set(AMP_DISABLE);
            pshifter_state(PSHIFTER_ENABLE);
            pamp_set(AMP_ENABLE);
            break;
        case STANDBY_MODE:
            lna_set(AMP_DISABLE);
            pshifter_state(PSHIFTER_DISABLE);
            pamp_set(AMP_DISABLE);
            switch_set(RXPATH);
            break;
        default:
            break;
    }
}

void process_message(message *msg)
{
    switch(msg->cmd) {
        case CMD_DAC_READ:
            msg->re_len = 2;
            msg->response = 0;
            break;
        case CMD_DAC_SET:
            phase_raw(msg->payload);
            msg->re_len = 0;
            break;
        case CMD_DET_READ:
            msg->re_len = 2;
            msg->payload = pamp_readpower();
            break;
        case CMD_DAC_THERM_READ:
            msg->re_len = 2;
            msg->payload = 0; // NO LONGER USED
            break;
        case CMD_ATT_READ:
            msg->re_len = 2;
            msg->payload = att_get();
            break;
        case CMD_ATT_SET:
            msg->re_len = 0;
            att_set(msg->payload & ATT_LEVELMASK); 
            break;
        case CMD_ATT_SET_RAW:
            msg->re_len = 0;
            att_set_raw(msg->payload & ATT_LEVELMASK); 
            break;
        case CMD_PHASE_READ:
            msg->re_len = 2;
            msg->payload = phase_get();
            break;
        case CMD_PHASE_SET:
            msg->re_len = 0;
            phase_set(msg->payload);
            break;
        case CMD_PHASE_SET_RAW:
            msg->re_len = 0;
            phase_raw(msg->payload);
            break;
        case CMD_MODE_READ: // COMMAND UNIMPLEMENTED
            msg->re_len = 2;
            //uint8_t amp_state = ((AMP_PORT & (1 << AMP_EN)) >> AMP_EN);
            //uint8_t switch_state = SWITCH_PORT & ((1 << CS1) | (1 << CS2));
            //msg.payload = (amp_state << 8) | switch_state;
            break;
        case CMD_MODE_SET:
            msg->re_len = 0;
            set_mode(msg->payload);
            break;
        case CMD_PING:
            msg->re_len = 0;
            break;
        case CMD_DISP: // THIS TARGET DOES NOT IMPLEMENT DISP
            msg->valid = 0;
            break;
        case CMD_RESET:
            init_pshifter();
            msg->re_len = 0;
            break;
        default:
            msg->valid = 0;
            break;

    }
}

int main(void)
{
    init_pshifter();
    message msg;
    msg.destaddr = INVALID;

    while(1) {
        //WDTCTL = WDTPW + WDTCNTCL;
        rs485_get_message(&msg);
        if(msg.destaddr == BOARD_ADDR) {
            LEDOUT |= LED1;
            process_message(&msg);
            delay_ms(20);
            rs485_send_message(&msg);
        }
        
    }

    return 0;
}

