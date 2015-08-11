
/*
*------------------------------------------------------------------------------
* main.c
*
*------------------------------------------------------------------------------
*/


#pragma config FOSC 	= INTIO67
#pragma config PLLCFG 	= OFF
#pragma config PRICLKEN = OFF
#pragma config FCMEN 	= OFF
#pragma config IESO 	= OFF
#pragma config PWRTEN   = OFF
#pragma config BOREN 	= OFF
#pragma config BORV 	= 285
#pragma config WDTEN    = OFF
#pragma config WDTPS 	= 512
#pragma config CCP2MX 	= PORTC1
#pragma config PBADEN 	= OFF
#pragma config MCLRE 	= EXTMCLR
#pragma config STVREN 	= OFF
#pragma config XINST 	= OFF
#pragma config LVP      = OFF
#pragma config CP0      = OFF
#pragma config CP1      = OFF
#pragma config CP2      = OFF
#pragma config CP3      = OFF
#pragma config CPB      = OFF
#pragma config CPD      = OFF
#pragma config WRT0     = OFF
#pragma config WRT1     = OFF
#pragma config WRT2     = OFF
#pragma config WRT3     = OFF
#pragma config WRTB     = OFF       
#pragma config WRTC     = OFF
#pragma config WRTD     = OFF
#pragma config EBTR0    = OFF
#pragma config EBTR1    = OFF
#pragma config EBTR2    = OFF
#pragma config EBTR3    = OFF
#pragma config EBTRB    = OFF





/*
*------------------------------------------------------------------------------
* Include Files
*------------------------------------------------------------------------------
*/

#include <timers.h>				// Timer library functions
#include <delays.h>				// Delay library functions
#include <string.h>				// String library functions
#include "board.h"				// board setup
#include "timer.h"
#include "heartbeat.h"
#include "config.h"
#include "uart.h"
#include "communication.h"
#include "app.h"
/*
*------------------------------------------------------------------------------
* Private Defines
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Processor config bits
*------------------------------------------------------------------------------
*/



/*
*------------------------------------------------------------------------------
* Private Macros
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Data Types
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Variables
*------------------------------------------------------------------------------
*/


/*

*------------------------------------------------------------------------------
* Private Variables (static)
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Constants
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Constants (static)
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Function Prototypes (static)
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Functions
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Functions
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* void main(void)

* Summary	: Application specifc main routine. Initializes all port and
*			: pheriperal and put the main task into an infinite loop.
*
* Input		: None
*
* Output	: None
*
*------------------------------------------------------------------------------
*/


void main(void)
{
	unsigned long temp;

	BRD_init();			//board initialization

	TMR0_init(tickPeriod,0);		//initialize timer0

//	TMR1_init(tickPeriod,0);		//initialize timer0

	// initialise communication  module
	COM_init(CMD_SOP,CMD_EOP,RESP_SOP,RESP_EOP );
	COM_register(0 ,APP_comCallBack_1 ); 
	COM_register(1 ,APP_comCallBack_2 );

	EnableInterrupts();		//Interrupts initialization

	//Heart Beat to blink at every 500ms
	temp = (500UL *1000UL)/TIMER0_TIMEOUT_DURATION;
	
	while(1)
	{

		//Heart Beat to blink at every 500ms
	`	if(heartBeatCount >= temp )
		{			
			HB_task();
			heartBeatCount = 0;
		}

		if(appUpdateCount >= 10)
		{
			APP_task();
			appUpdateCount = 0;
		}
	//	if(comUpdateCount >= 25)
	//	{
			COM_task();
	//		comUpdateCount = 0;
	//	}
	}


}

/*
*  End of main.c
*/
