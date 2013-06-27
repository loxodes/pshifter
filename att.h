void init_att(void);
void att_clear(void);
void att_set(uint8_t level);
void att_set_raw(uint8_t level);
void att_send(uint16_t w);
uint8_t att_get(void);

#define A0 (1 << 0)
#define A1 (1 << 1)
#define A2 (1 << 2)

#define ATT_ADDR_BASE 8
#define ATT_LEVELMASK 0x7F
