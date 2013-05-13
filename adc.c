// jon klein, jtklein@alaska.edu
// MIT license
// controls MSP430 built in ADC 

#include <msp430.h>
#include <stdint.h>
#include "pins.h"
#include "adc.h"

void init_adc(void)
{
    // MCLK = 16 MHz, ADC clock is 16MHz / 8 / 64 ~= 31.25 kHz
    ADC10CTL0 &= ~(ENC);
    ADC10CTL0 = ADC10SHT_3 | ADC10ON | REFON | REF2_5V | SREF_1;
}

// single conversion blocking ADC
uint16_t adc_read(uint16_t channel)
{
    ADC10CTL0 &= ~(ENC);
    ADC10CTL1 = ADC10SSEL_0 | ADC10DIV_3 | channel;
    ADC10CTL0 |= ENC + ADC10SC;
    while(ADC10CTL0 & ADC10BUSY)
    {
        ;
    }
    return ADC10MEM;
}
