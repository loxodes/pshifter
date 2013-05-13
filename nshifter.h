#define BOARD_ADDR 'f'

void init_msp(void);
void init_gpio(void);
void init_leds(void);
void init_pshifter(void);

void set_mode(uint16_t mode);
void process_message(message *msg);
