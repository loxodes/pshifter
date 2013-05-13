#define INCOMING 1
#define OUTGOING 0

#define VALID 1
#define INVALID 0

#define SET_CMD 1
#define GET_CMD 0

#define MSGBUFFERSIZE 4
#define ACK 0x06
#define NAK 0x15
#define STX 0x02
#define ETX 0x03
#define ENQ 0x05
#define RS 0x1E
#define SOH 0x01

#define STANDBY_MODE 0 
#define RECEIVE_MODE 1 
#define TRANSMIT_MODE 2 

// for fail, send [NAK] [CMD] [ETX]
// for 16 bit values, send LSByte, MSbyte

#define CMD_DAC_READ 0xD0
// DAC_READ reads the value of the phase shifter DAC
// send [STX] [CMD_DAC_READ] [ETX]
// get [ACK] [CMD] [2] [VAL (16 bit)] [ETX]

#define CMD_DAC_SET 0xD1
// DAC_SET sets the value of a phase shifter DAC 
// send [STX] [CMD_DAC_WRITE] [2] [VAL (16 bit)] [ETX]
// get [ACK] [CMD] [0] [ETX]

#define CMD_DET_READ 0xD2
// DET_READ reads the value of amplifier detect 
// send [STX] [CMD_DAT_READ] [0] [ETX]
// get [ACK] [CMD] [2] [VAL (16 bit)] [ETX]

#define CMD_DAC_THERM_READ 0xD4
// DAC_THERM reads the state of the DAC thermal overload
// send [STX] [CMD_THERM_READ] [0] [ETX]
// get [ACK] [CMD] [state (16 bit)] [ETX]

#define CMD_ATT_READ 0xD6
// ATT_READ reads the value of the attenuator
// send [STX] [CMD_ATT_READ] [0] [ETX]
// get [ACK] [CMD] [2] [VAL (16 bit)] [ETX]
#define CMD_ATT_SET 0xD7
// ATT_SET sets the value of a ATT channel
// send [STX] [CMD_ATT_SET]  [2] [VAL (16 bit)] [ETX]
// get [ACK] [CMD] [0] [ETX]

#define CMD_PHASE_READ 0xD8
// PHASE_READ reads the value of a channel
// send [STX] [CMD_ATT_READ] [0] [ETX]
// get [ACK] [CMD] [2] [VAL (16 bit)] [ETX]

#define CMD_PHASE_SET 0xD9
// PHASE_SET sets the value of a channel
// send [STX] [CMD_ATT_SET] [2] [VAL (16 bit)] [ETX]
// get [ACK] [CMD] [0] [ETX]

#define CMD_MODE_READ 0xDC
// PHASE_READ reads the device mode (TX/RX/NA)
// send [STX] [CMD_MODE_READ] [0] [ETX]
// get [ACK] [CMD] [2] [VAL (16 bit)] [ETX]

#define CMD_MODE_SET 0xDD
// SW_SET sets device mode (TX/RX)
// send [STX] [CMD_MODE_SET]  [2] [VAL (16 bit)] [ETX]
// get [ACK] [CMD] [0] [ETX]

#define CMD_DISP 0xDE
// escape sequence for transmitting strings
// send [STX] [CMD_DISP] [N] [N characters] [ETX]
// get [ACK] [CMD] [0] [ETX]

#define CMD_RESET 0xDF
// reset device
// send [STX] [CMD_RESET] [0] [ETX]
// get [ACK] [CMD] [0] [ETX]

#define CMD_PING 0xE0
// ping device 
// send [STX] [CMD_PHC_PING] [0] [ETX]
// get [ACK] [CMD] [0] [ETX]

//                                                     [-----------LEN---------------]
// format [ENQ] [TADDR] [RS] [SADDR] [STX] [SOH] [CMD] [LEN] [PAYLOAD, FIELDS SOH SEPARATED] [ETX]
// ack	  [ENQ] [SADDR] [RS] [TADDR] [STX] [ACK] [CMD] [LEN] [PAYLOAD, FIELDS SOH SEPARATED] [ETX]
// nak	  [ENQ] [SADDR] [RS] [TADDR] [STX] [NAK] [CMD] [ETX]

// all addr are 16 bit

typedef struct {
    uint16_t sourceaddr;
    uint16_t destaddr;
    uint8_t cmd;
    uint8_t dir;
    uint8_t len;
    uint8_t re_len;
    uint8_t valid;
    uint16_t payload;
    uint16_t response;
} message;


