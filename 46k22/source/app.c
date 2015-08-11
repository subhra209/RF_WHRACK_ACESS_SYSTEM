
/*
*------------------------------------------------------------------------------
* Include Files
*------------------------------------------------------------------------------
*/
#include "config.h"
#include "board.h"
#include "timer.h"
#include "communication.h"
#include "string.h"
#include "eep.h"


//#define SIMULATION
/*
*------------------------------------------------------------------------------
* Structures
*------------------------------------------------------------------------------
*/																	

typedef struct _APP
{
	UINT8 log[MAX_CARDS][10];  			//store for all scanned card enty
	UINT8 input[MAX_INPUT][10];					//store the card number that scaned 
	UINT8 inputRecieved[MAX_INPUT];				// flag indicaation for card scanned
	UINT8 logIndex;						//maintain log position






}APP;																			



/*
*------------------------------------------------------------------------------
* Variables
*------------------------------------------------------------------------------
*/
#pragma idata APP_DATA
APP app = {0};
#pragma idata


/*------------------------------------------------------------------------------
* Private Functions
*------------------------------------------------------------------------------
*/


/*
*------------------------------------------------------------------------------
* void APP-init(void)
*------------------------------------------------------------------------------
*/

void APP_init(void)
{

}




/*
*------------------------------------------------------------------------------
* void APP-task(void)
*------------------------------------------------------------------------------
*/
void APP_task(void)
{

	UINT8 i ;
	UINT8 inputIndex;
	BOOL newCardFlag = TRUE;
	UINT8 cardEntry = 0 ;
	
	for(inputIndex = 0 ; inputIndex < MAX_INPUT ; inputIndex++)
	{
		if( app.inputRecieved[inputIndex] == TRUE)
		{
	
			for( i = 0 ; i < MAX_CARDS ; i++)
			{
				if (strcmp(app.input[inputIndex],app.log[i]) == 0 )
				{
					memset(app.log[i],'\0',10);
					newCardFlag = FALSE;
				}
	
			} 
			
			if(newCardFlag == TRUE)
			{
				if( app.logIndex >= MAX_CARDS)
					app.logIndex = 0;
	
				strcpy(app.log[app.logIndex],app.input[inputIndex] );
				app.logIndex++;
			}
			app.inputRecieved[inputIndex] = FALSE;
			
		}
	}
	for( i = 0 ; i < MAX_CARDS ; i++)
	{
		if(app.log[i][0] != '\0')
			cardEntry++ ;
	} 

	switch (cardEntry)
	{
		case 0:
				BULB = SWITCH_OFF;
				Delay10us(10);
			 	HOTTER = SWITCH_OFF;
			break;
		case 1:
				BULB = SWITCH_ON;
			 	HOTTER = SWITCH_OFF;
			break;
		default:
	 			HOTTER	= SWITCH_ON;
			break;
	}
	

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
*/
	strcpy(app.input[0], rxPacket );
	app.inputRecieved[0] = TRUE;

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
*/
	strcpy(app.input[1], rxPacket );
	app.inputRecieved[1] = TRUE;

	return length;


}

	
		