#ifndef CONFIG_H
#define CONFIG_H

/*
*------------------------------------------------------------------------------
* config.h
*
*/


#define DEVICE_ADDRESS			0x01

#define __DISPLAY_TEST__ 


//MMD module configuration
#define MMD_MAX_CHARS		16
#define MMD_MAX_SEGMENTS	2
#define MMD_MAX_ADDRESS		128
//#define __MMD_STATIC__

//#define _MMD_TEST_
//#define COMMON_CATHODE
/*----------------------------------------
*	TIMER Configuration
*----------------------------------------*/

#define TIMESTAMP_DURATION 		(200)			

/*----------------------------------------
*	USART Configuration
*----------------------------------------*/
#define ACTIVE_USARTS		2
#define UART2_ACTIVE
#define UART1_BAUD			19200
#define UART2_BAUD			19200
//#define PASS_THROUGH
#define UART_TEST

/*----------------------------------------
*	COM module configuration
*----------------------------------------*/
#define PACKET_LENGTH 	6
#define __NO_CHECKSUM__
#define __BCC_XOR__
//#define __RESPONSE_ENABLED__
#define __LOOP_BACK__
#define BROADCAST_ADDRESS		0xFF
#define CMD_SOP	0xAA
#define CMD_EOP 0xBB
#define RESP_SOP	0xCC
#define RESP_EOP	0xDD


enum
{
	COM_RX_PACKET_SIZE = 10,
	COM_TX_PACKET_SIZE = 10
};

#define 	RX_PACKET_SIZE		(60)	
#define 	TX_PACKET_SIZE		(60)


/*----------------------------------------
*	Keypad Configurations
*----------------------------------------*/

//#define __FACTORY_CONFIGURATION__
//#define __SIMULATION__
/*----------------------------------------
*	App Configurations
*----------------------------------------*/





//RTC CONFIGURATION
//#define TIME_DEBUG

//#define RTC_DS1307
#define RTC_DS3232

//#define __SET_RTC__


//APPLICATION CONFIGURATION
#define MSG_MAX_CHARS 60
#define MAX_TRANSITIONS 20

//#define __ERROR_DEBUG__

/*----------------------------------------
*	APP CONFIGURATION
*----------------------------------------*/
#define MSG_LENGTH 		20
#define EEPROM_ADDRESS 	(0X10)


#define EPROM_ADD_PLAN  	0
#define EPROM_ADD_ACTUAL 	4
#define SET_PLAN 			0x81
#define MODIFY_PLAN 		0x82

#define NO_OF_DIGITS		(0x04)
#define MAX_COUNT			(9999)

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



