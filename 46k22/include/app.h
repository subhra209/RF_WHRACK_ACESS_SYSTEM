#ifndef _APP_H_
#define _APP_H_


/*
*----------------------------------------------------------------------------------------------------------------
*	MACROS
*-----------------------------------------------------------------------------------------------------------------
*/

//#define __FACTORY_CONFIGURATION__


/*
*----------------------------------------------------------------------------------------------------------------
*	Enumerations
*-----------------------------------------------------------------------------------------------------------------
*/


enum
{
	CMD_GET_STATUS = 0x80,
	CMD_GET_ADMIN_PASSWORD = 0x81,
	CMD_GET_LOGON_PASSWORD = 0x82,
	CMD_GET_BUZZER_TIMEOUT = 0x83
	
};


UINT8 APP_comCallBack_1( far UINT8 *rxPacket,  far UINT8* txCode, far UINT8** txPacket);
UINT8 APP_comCallBack_2( far UINT8 *rxPacket,  far UINT8* txCode, far UINT8** txPacket);
extern void APP_init(void);
extern void APP_task(void);



#endif