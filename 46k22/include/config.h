#ifndef CONFIG_H
#define CONFIG_H

/*
*------------------------------------------------------------------------------
* config.h
*
*/

/*----------------------------------------
*	BOARD MOULE CONFIG
*----------------------------------------*/

#define MHz_64					(64000000UL)	// Hz
#define MHz_32					(32000000UL)	// Hz
#define MHz_16					(16000000UL)	// Hz
#define MHz_10					(10000000UL)	// Hz

/*----------------------------------------
*	OSCILLATOR CONFIG
*----------------------------------------*/

#define SYSTEM_CLOCK			(MHz_16)	


/*----------------------------------------
*	TIMER CONFIG
*----------------------------------------*/
#define FULLSCALE_16BIT				(65535)
#define TIMER0_TIMEOUT_DURATION 	(1000UL)			//1ms
#define TIMESTAMP_DURATION 		(200)			

/*----------------------------------------
*	DEVICE CONFIG
*----------------------------------------*/
#define __FACTORY_CONFIGURATION__
#define DEVICE_ADDRESS			0xFA


/*----------------------------------------
*	USART Configuration
*----------------------------------------*/
#define ACTIVE_USARTS		2
#define UART2_ACTIVE
#define UART1_BAUD			9600
#define UART2_BAUD			9600
//#define PASS_THROUGH
//#define UART_TEST

/*----------------------------------------
*	COM module configuration
*----------------------------------------*/
#define PACKET_LENGTH 	6
#define __BCC_XOR__
//#define __RESPONSE_ENABLED__
//#define __LOOP_BACK__
#define BROADCAST_ADDRESS		0xFF
#define CMD_SOP	0x41
#define CMD_EOP 0xBB
#define RESP_SOP	0xCC
#define RESP_EOP	0xDD
#define __NO_CHECKSUM__

enum
{
	COM_RX_PACKET_SIZE = 25,
	COM_TX_PACKET_SIZE = 25
};

#define 	RX_PACKET_SIZE		(60)	
#define 	TX_PACKET_SIZE		(60)

/*----------------------------------------
*	Display module configuration
*----------------------------------------*/
#define __DISPLAY_TEST__
#define MAX_MSG 7
//#define COMMON_CATHODE

/*----------------------------------------
*	MMD module configuration
*----------------------------------------*/
#define MMD_MAX_CHARS		8
#define MMD_MAX_SEGMENTS	4
#define MMD_MAX_ADDRESS		128
#define __MMD_STATIC__

/*----------------------------------------
*	RTC CONFIGURATION
*----------------------------------------*/

//#define TIME_DEBUG

//#define RTC_DS1307
//#define RTC_DS3232

/*----------------------------------------
*	Keypad Configurations
*----------------------------------------*/

//#define __SIMULATION__

/*----------------------------------------
*	APP Configurations
*----------------------------------------*/
#define MAX_CARDS	10
#define MAX_INPUT   2


// Enable for external eeprom access
// Comment out this line if internal eeprom access required
#define EEP_EXTERRNAL
// For uart enable
//#define EN_UART
// Enable for hardware i2c module usage
//#define USE_MSSP_I2C



/*
*------------------------------------------------------------------------------
* Public Data Types
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Variables (extern)
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Constants (extern)
*------------------------------------------------------------------------------
*/


/*
*------------------------------------------------------------------------------
* Public Function Prototypes (extern)
*------------------------------------------------------------------------------
*/

#endif
/*
*  End of config.h
*/



