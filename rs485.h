void init_rs485(void);
uint16_t rs485_send_message(message *msg);
void rs485_get_message(message *msg);
uint8_t rs485_mode(uint8_t mode);

#define RS485_READ 2
#define RS485_SEND 3
#define RS485_STANDBY 4

#define MAXPAYLOAD 4
