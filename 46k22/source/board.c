
/*
*------------------------------------------------------------------------------
* device.c
*
* Board specipic drivers module(BSP)
*
*
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*------------------------------------------------------------------------------
*/


/*
*------------------------------------------------------------------------------
* Include Files
*------------------------------------------------------------------------------
*/


#include "board.h"
#include "config.h"
#include "typedefs.h"




/*
*------------------------------------------------------------------------------
* Private Defines
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
UINT16 tickPeriod = 0;
/*
*------------------------------------------------------------------------------
* Private Variables (static)
*------------------------------------------------------------------------------
*/

static BOOL ledState;

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
* void InitializeBoard(void)

* Summary	: This function configures all i/o pin directions
*
* Input		: None
*
* Output	: None
*
*------------------------------------------------------------------------------
*/
void BRD_init(void)
{
	unsigned long clock, temp;

	//Store sytem clock
	clock = SYSTEM_CLOCK;

	switch(clock)
	{
		case MHz_64:
		default:
				OSCCON = 0X70; 		//internal oscillator 64MHz
				OSCTUNEbits.PLLEN = 1;	//PLL Enable
		break;

		case MHz_32:
				OSCCON = 0X60; 		//internal oscillator 32MHz
				OSCTUNEbits.PLLEN = 1;	//PLL Enable
		break;
		
		case MHz_16:
				OSCCON = 0X70; 		//internal oscillator 16MHz
				OSCTUNEbits.PLLEN = 0;	//PLL Enable
		break;
	}

	//calculating tick period for timer-0
	temp = clock >> 2;	
	temp *= (TIMER0_TIMEOUT_DURATION/1000);
	temp /= 1000;		

	tickPeriod = (FULLSCALE_16BIT - (UINT16)temp);



	// set all anolog channels as Digital I/O
	ADCON0 = 0x00;	
	ANSELA = 0;
	ANSELB = 0;
	ANSELC = 0;
	ANSELD = 0;
	ANSELE = 0;

	HEART_BEAT_DIRECTION = PORT_OUT;

	// Rs485 Direction Controle
	TX1_EN_DIR 		= PORT_OUT;
	TX1_EN 			= SWITCH_OFF;

	TX2_EN_DIR 		= PORT_OUT;
	TX2_EN 			= SWITCH_OFF;

	// Configure Serial port
	SER2_TX_DIR 		=  PORT_OUT;
	SER2_RX_DIR 		=  PORT_IN;

	SER1_TX_DIR 		=  PORT_OUT;
	SER1_RX_DIR 		=  PORT_IN;


	BULB_DIR			=  PORT_OUT;
	HOTTER_DIR			=  PORT_OUT;

	BULB				= SWITCH_OFF;
 	HOTTER				= SWITCH_OFF;

    // Enable internal PORTB pull-ups
    INTCON2bits.RBPU = 0;

	

}



/*
*------------------------------------------------------------------------------
* Private Functions
*------------------------------------------------------------------------------
*/

/*
*  End of device.c
*/
