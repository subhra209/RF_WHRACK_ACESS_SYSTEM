
#ifndef __COMMUNICATION__
#define __COMMUNICATION__

/*
*------------------------------------------------------------------------------
* Include Files
*------------------------------------------------------------------------------
*/

#include "board.h"
#include "config.h"
#include "uart.h"
#include "timer.h"
#include <usart.h>

/*
*------------------------------------------------------------------------------
* Private Enumeration
*------------------------------------------------------------------------------
*/
typedef enum
{
	COM_RESP_NONE = 0xFF,
	COM_RESP_OK = 0,
	COM_RESP_OVERRUN = 0xF0,
	COM_RESP_CHECKSUM_ERROR = 0xF1,
	COM_RESP_INVALID_CMD = 0xF2, 
	COM_RESP_INVALID_DATA = 0xF3
}COM_RESP_CODE;

enum
{
	IGNORE = 0,
	PARSE_SUCCESS = 1,
	PARSE_FAILURE = 2
};

enum
{
	TIMEOUT = 65535
};


enum 
{ 
	COM_RESET = 0,
	COM_START = 1,
	COM_IN_PACKET_COLLECTION = 2,
	COM_IN_TX_DATA = 3
};



enum 
{
	COM_DEVICE_ADDRESS_INDEX = 0,
	COM_TX_CODE_INDEX = 1,
	COM_TX_DATA_START_INDEX = 2,
	COM_RX_DATA_START_INDEX = 0
};

enum
{
	UART1 = 0,
	UART2
};

/*
*------------------------------------------------------------------------------
*  structures 
*------------------------------------------------------------------------------
*/

typedef struct _COMMUNICATTION
{
	
	UINT8 state;
	UINT8 rx_sop;
	UINT8 rx_eop;
	UINT8 tx_sop;
	UINT8 tx_eop;
	UINT8 rxPacketBuffer[COM_RX_PACKET_SIZE];
	UINT8 txPacketBuffer[COM_TX_PACKET_SIZE];
	UINT8 rxPacketIndex;
	UINT8 txPacketLength;
	UINT8 txCode;
	UINT8 timeout;
	UINT8 (*callBack)(UINT8* cmd_data, UINT8* respID,  UINT8** resp_data);

	UINT32 prevAppTime, curAppTime;
	UINT8 prevState ;
}COMMUNICATION;


/*
*------------------------------------------------------------------------------
* Private Functions	Prototypes
*------------------------------------------------------------------------------
*/

UINT8 COM_BCC( UINT8* data  , UINT8 length);
UINT8 checksum();
UINT8 parse1Packet(UINT8 *respCode, UINT8 uart);
UINT8 parse2Packet(UINT8 *respCode);
void COM_reset(void);
void COM_txData(void);
void COM2_txData(void);
UINT8 checksum(UINT8 *buffer, UINT8 length);
void COM1_restart(void);


/*
*------------------------------------------------------------------------------
* Public Functions	Prototypes
*------------------------------------------------------------------------------
*/
void COM_init(UINT8 cmd_sop , UINT8 cmd_eop ,UINT8 resp_sop , UINT8 resp_eop );
void COM_register(UINT8 uart , 	UINT8 (*callBack)(UINT8* cmd_data, UINT8* respID,  UINT8** resp_data) ); 

void COM_txStr(rom UINT8 *str);
UINT8 COM_txBuffer(far UINT8 *txData, UINT8 length);
void COM_txCMD_CHAN1(UINT8 deviceAddress, 
			UINT8 cmd, far UINT8 *buffer , UINT8 length);
void COM1_reset(void);
void COM2_reset(void);
void COM1_task(void);
void COM_task(void);


#endif


