#define LEDOUT P1OUT
#define LEDDIR P1DIR
#define LED1 (1 << 3)

#define GPIOOUT P1OUT
#define GPIODIR P1DIR
#define GP0 (1 << 4)
#define GP1 (1 << 4)
#define GP2 (1 << 5)

#define PSHIFTOUT P1OUT
#define PSHIFTDIR P1DIR
#define PSHIFT_EN (1 << 1)
#define PSHIFT_SYNC (1 << 7)

#define AMPENOUT P1OUT
#define AMPENDIR P1DIR
#define PAMP_EN (1 << 0)

#define LNAENOUT P2OUT
#define LNAENDIR P2DIR
#define LNA_EN (1 << 3)

#define AMPDETOUT P1OUT
#define AMPDETSEL P1SEL
#define AMPDETDIR P1DIR
#define AMPDETAE ADC10AE0
#define AMP_DET (1 << 4)
#define AMP_DET_ADCCHAN 4

#define RFSWITCHOUT P3OUT
#define RFSWITCHDIR P3DIR

#define TXPATH (0 << 6)
#define RXPATH (1 << 6)

#define SELPATH (1 << 6)
#define INVPATH (1 << 7)

#define UARTOUT P3OUT
#define UARTSEL P3SEL
#define UARTDIR P3DIR
#define RXD (1 << 5)
#define TXD (1 << 4)

#define SPIOUT P3OUT
#define SPISEL P3SEL
#define SPIDIR P3DIR

#define SCK (1 << 3)
#define MISO (1 << 2)
#define MOSI (1 << 1)

#define ATTDIR P1DIR
#define ATTOUT P1OUT
#define ATT_LE (1 << 6)

#define RS485OUT P2OUT
#define RS485DIR P2DIR
#define RE (1 << 1)
#define DE (1 << 2)

