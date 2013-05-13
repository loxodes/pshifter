void init_phase(void);
void phase_set(uint16_t p);
void phase_raw(uint16_t p);
void pshifter_state(uint16_t s);

uint16_t phase_get(void);

#define MAPS_MASK 0x3F
#define PSHIFTER_ENABLE 1
#define PSHIFTER_DISABLE 0
