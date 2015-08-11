
#include "string.h"
#include "app.h"
#include "typedefs.h"
#include "eep.h"






/*
*------------------------------------------------------------------------------
* app - the app structure. 
*------------------------------------------------------------------------------
*/
typedef struct _APP
{
	//Variables to handle dot matrix display
	UINT8 model[MSG_LENGTH];
	UINT8 eepUpdate;

}APP;

#pragma idata APP_DATA
APP app = {{0},0};

#pragma idata



/*
*------------------------------------------------------------------------------
* void APP_init(void)
*
* Summary	: Initialize application
*
* Input		: None
*
* Output	: None
*------------------------------------------------------------------------------
*/

void APP_init(void)
{

}

/*
*------------------------------------------------------------------------------
* void APP_task(void)
*
* Summary	: 
*
* Input		: None
*
* Output	: None
*------------------------------------------------------------------------------
*/

void APP_task(void)
{




}





UINT8 APP_comCallBack_1( far UINT8 *rxPacket, far UINT8* txCode,far UINT8** txPacket)
{
	UINT8 i;
	UINT8 length = 0;

	//copy data rxbuffer to app buffer
/*
	for(i = 0 ; i < 6 ; i++)
	{
		app.input[i] = rxPacket[i];
	}

	strcpy(app.input[0], rxPacket );
	app.inputRecieved[0] = TRUE;
*/
	return length;


}
UINT8 APP_comCallBack_2( far UINT8 *rxPacket, far UINT8* txCode,far UINT8** txPacket)
{
	UINT8 i;
	UINT8 length = 0;

	//copy data rxbuffer to app buffer
/*
	for(i = 0 ; i < 6 ; i++)
	{
		app.input[i] = rxPacket[i];
	}

	app.input[i] = '\0';

	strcpy(app.input[1], rxPacket );
	app.inputRecieved[1] = TRUE;
*/
	return length;


}
