#define eint() __bis_status_register(GIE)
#define dint() __bic_status_register(GIE)
#define interrupt(x) void __attribute__((interrupt (x))) 

