
/*
*------------------------------------------------------------------------------
* Include Files
*------------------------------------------------------------------------------
*/

#include "communication.h"


/*
*------------------------------------------------------------------------------
* pragma Declaration
*------------------------------------------------------------------------------
*/
#pragma idata COM_DATA
COMMUNICATION communication[ACTIVE_USARTS] = {0};
#pragma idata

/*
*------------------------------------------------------------------------------
* public variables
*------------------------------------------------------------------------------
*/
UINT16 comTimeout = 0xFFFF;
UINT8 rom alert[]={"COM\n"};




void COM_init(UINT8 rx_sop , UINT8 rx_eop ,UINT8 tx_sop , UINT8 tx_eop )
{
	int i;

	//UART1_init();	//initialize uart

	for( i = 0 ; i < ACTIVE_USARTS ; i++)
	{
		communication[i].rx_sop = rx_sop;
		communication[i].rx_eop = rx_eop;
		communication[i].tx_sop = tx_sop;
		communication[i].tx_eop = tx_eop;
	}

	COM1_restart();
}


void COM_register (UINT8 uart , 	UINT8 (*callBack)(UINT8* cmd_data, UINT8* respID,  UINT8** resp_data))
{
		communication[uart].callBack = callBack;
}


void COM1_restart()
{
	UINT8 i;
#if (defined __18F8722_H) ||(defined __18F46K22_H)
	UART1_init(UART1_BAUD);
#ifdef UART2_ACTIVE
	UART2_init(UART2_BAUD);
#endif
#else
	UART_init();	//initialize uart
#endif

	for( i = 0 ; i < ACTIVE_USARTS ; i++)
	{
		communication[i].rxPacketIndex = 0;
		communication[i].txPacketLength = 0;
		communication[i].state = COM_START;
		communication[i].txCode = IGNORE;
		communication[i].timeout = TIMEOUT;
		communication[i].prevAppTime = communication[i].curAppTime;
		communication[i].prevState = communication[i].state;
	}

}


void COM1_reset()
{
		communication[0].rxPacketIndex = 0;
		communication[0].txPacketLength = 0;
		communication[0].state = COM_START;
		communication[0].txCode = IGNORE;
		communication[0].timeout = TIMEOUT;
		communication[0].prevAppTime = communication[0].curAppTime;
		communication[0].prevState = communication[0].state;

}

void COM2_reset()
{
		communication[1].rxPacketIndex = 0;
		communication[1].txPacketLength = 0;
		communication[1].state = COM_START;
		communication[1].txCode = IGNORE;
		communication[1].timeout = TIMEOUT;
		communication[1].prevAppTime = communication[1].curAppTime;
		communication[1].prevState = communication[1].state;
}

#ifdef __LOOP_BACK__
void COM_task()
{
	UINT8 uartData = 0;

	if(RCSTA1bits.OERR == 1)
	{
		RCSTA1bits.CREN = 0;
		Delay10us(1);
		RCSTA1bits.CREN = 1;
	}
#ifdef  UART2_ACTIVE

	if(RCSTA2bits.OERR == 1)
	{
		RCSTA2bits.CREN = 0;
		Delay10us(1);
		RCSTA2bits.CREN = 1;
	}
#endif

#if(defined __18F8722_H) ||(defined __18F46K22_H)
	if( UART1_hasData() )
	{
		uartData = UART1_read();	

		UART1_write(uartData);
		UART1_transmit();
	}
#else
	if( UART_hasData() )
	{
		uartData = UART_read();	

		UART_write(uartData);
		UART_transmit();
		return;

	}

#endif

#ifdef UART2_ACTIVE

	if( UART2_hasData() )
	{
		uartData = UART2_read();
		UART2_write(uartData);
	//	UART2_transmit();
	}

#endif
		return;



}

#else

void COM_task(void)
{
	UINT8 j;
	UINT8 channel;
	volatile UINT8 uartData = 0;

	if(RCSTA1bits.OERR == 1)
	{
		RCSTA1bits.CREN = 0;
		Delay10us(1);
		RCSTA1bits.CREN = 1;
	}
#ifdef  UART2_ACTIVE

	if(RCSTA2bits.OERR == 1)
	{
		RCSTA2bits.CREN = 0;
		Delay10us(1);
		RCSTA2bits.CREN = 1;
	}
#endif

for(channel = 0 ; channel < ACTIVE_USARTS ; channel++ )
{
	communication[channel].curAppTime = GetAppTime();
	if( communication[channel].prevAppTime != communication[channel].curAppTime)
	{
		if( communication[channel].prevState == communication[channel].state && (communication[channel].state == COM_IN_PACKET_COLLECTION))
		{
			--communication[channel].timeout ;
			if( communication[channel].timeout == 0)
			{
				COM1_restart();
				break;
			}
			
		}
		
		communication[channel].prevAppTime = communication[channel].curAppTime;
	}

	switch( communication[channel].state)
	{
		case COM_START:
		if( channel == UART1)
		{
			if( UART1_hasData() == FALSE )
					break;
			
				uartData = UART1_read();
			#ifdef	PASS_THROUGH
				UART2_write(uartData);
				UART2_transmit();
			#endif	

		}
		else if( channel == UART2)
		{
			if( UART2_hasData() == FALSE )
				break;

			uartData = UART2_read();	

		}
			
		if( uartData == communication[channel].rx_sop )
		{
			communication[channel].rxPacketIndex = 0;
			communication[channel].state = COM_IN_PACKET_COLLECTION;
		}
		break;

		case COM_IN_PACKET_COLLECTION:

			if( channel == UART1)
			{
				if( UART1_hasData() == FALSE )
						break;
				
					uartData = UART1_read();
				#ifdef	PASS_THROUGH
					UART2_write(uartData);
					UART2_transmit();
				#endif	
	
			}
			else if( channel == UART2)
			{
				if( UART2_hasData() == FALSE )
					break;
	
				uartData = UART2_read();	
	
			}

			communication[channel].rxPacketBuffer[communication[channel].rxPacketIndex++]=uartData;
			communication[channel].timeout = 0;
			if( communication[channel].rxPacketIndex >= RX_PACKET_SIZE)
			{
				communication[channel].txPacketBuffer[COM_DEVICE_ADDRESS_INDEX] = DEVICE_ADDRESS;	//store device address
				++communication[channel].txPacketLength;

				communication[channel].txPacketBuffer[COM_TX_CODE_INDEX] = COM_RESP_OVERRUN;		//store tx code
				++communication[channel].txPacketLength;
				
				communication[channel].state = COM_IN_TX_DATA;
				
			}
		
			if(communication[channel].rxPacketIndex >= PACKET_LENGTH )
			{
				UINT8 *txData ;
				UINT8 parseResult = 0;
				COM_RESP_CODE txCode = COM_RESP_NONE;

				communication[channel].rxPacketBuffer[communication[channel].rxPacketIndex++] = '\0';


#ifdef __NO_CHECKSUM__
				parseResult = PARSE_SUCCESS;
#else				
				parseResult = parse1Packet((&txCode), channel);		//parse packet 
#endif

				switch( parseResult)
				{
					case IGNORE:
					COM1_reset();	
					break;
					
					case PARSE_SUCCESS:
											
					if( communication[channel].callBack != 0 )
					{
					//	communication[channel].state = COM_START;
						communication[channel].txPacketLength = 	communication[channel].callBack(&communication[channel].rxPacketBuffer[COM_RX_DATA_START_INDEX], 
																&communication[channel].txCode,&txData);
																							  
						communication[channel].txPacketBuffer[COM_DEVICE_ADDRESS_INDEX] = DEVICE_ADDRESS;	//store device address
						++communication[channel].txPacketLength;

						communication[channel].txPacketBuffer[COM_TX_CODE_INDEX] = communication[channel].txCode;	//store tx code
						++communication[channel].txPacketLength;

						for( j = COM_TX_DATA_START_INDEX ; j < communication[channel].txPacketLength ; j++)	//store data
						{
							communication[channel].txPacketBuffer[j] = *txData;
							txData++;
						}


					}

					else
					{
						COM1_reset();
					}

					break;
					
					case PARSE_FAILURE:
					{

						communication[channel].txPacketBuffer[COM_DEVICE_ADDRESS_INDEX] = DEVICE_ADDRESS;	//store device address
						++communication[channel].txPacketLength;
						
						communication[channel].txPacketBuffer[COM_TX_CODE_INDEX] = txCode;		//store tx code
						++communication[channel].txPacketLength;
						
					}
					
					break;
					
					default:
					break;
					}
					communication[channel].state = COM_IN_TX_DATA;
				}

			
				break;
	
			case COM_IN_TX_DATA:
	
			//	COM_txData();
				if(channel == UART1)
					COM1_reset();
				else if(channel == UART2)
					COM2_reset();
		
			break;
	
			default:

				if(channel == UART1)
					COM1_reset();
				else if(channel == UART2)
					COM2_reset();
			break;

		}
		communication[channel].prevState = communication[channel].state;

	}
}


#endif

UINT8 parse1Packet(UINT8 *respCode , UINT8 channel)
{

	UINT8 receivedChecksum = communication[channel].rxPacketBuffer[communication[channel].rxPacketIndex-1];
	UINT8 genChecksum = 0;


	if((communication[channel].rxPacketBuffer[ COM_DEVICE_ADDRESS_INDEX] != DEVICE_ADDRESS)
			&& (communication[channel].rxPacketBuffer[COM_DEVICE_ADDRESS_INDEX] != BROADCAST_ADDRESS) )
		return IGNORE;
	
	genChecksum = checksum(communication[channel].rxPacketBuffer,communication[channel].rxPacketIndex-1);
	
	if( receivedChecksum == genChecksum)
	{
		--communication[channel].rxPacketIndex;
		communication[channel].rxPacketBuffer[communication[channel].rxPacketIndex] = '\0'; //remove checksum from packet
	 
		return PARSE_SUCCESS;
	}
	else
	{	
		*respCode = COM_RESP_CHECKSUM_ERROR;
	 	return PARSE_FAILURE;
	}

}



void COM_txData()
{
	UINT8 bcc = 0;
	UINT8 i= 0;

	bcc = checksum(communication[0].txPacketBuffer, communication[0].txPacketLength);

#if(defined __18F8722_H) ||(defined __18F46K22_H)

	UART1_write(communication[0].tx_sop);

	for( i = 0; i < communication[0].txPacketLength; i++ )
	{
		UART1_write(communication[0].txPacketBuffer[i]);
	}

	UART1_write(bcc);
	UART1_write(communication[0].tx_eop);


#ifdef __RESPONSE_ENABLED__
	UART1_transmit();
#endif


#else 	//(defined __18F8722_H) ||(defined __18F46K22_H)
	
	UART_write(communication[0].tx_sop);

	for( i = 0; i < communication[0].txPacketLength; i++ )
	{
		UART_write(communication[0].txPacketBuffer[i]);
	}

	UART_write(bcc);
	UART_write(communication[0].tx_eop);


#ifdef __RESPONSE_ENABLED__
	UART_transmit();
#endif
	ClrWdt();

#endif
	
}







void COM_txStr(rom UINT8 *str)
{
#if(defined __18F8722_H) ||(defined __18F46K22_H)

	while(*str)
	{
		UART1_write(*str);
		str++;
	}
	UART1_transmit();

#else

	while(*str)
	{
		UART_write(*str);
		str++;
	}
	UART_transmit();
#endif
}


UINT8 checksum(UINT8 *buffer, UINT8 length)
{
	
	UINT8 bcc = 0;
	UINT8 i , j ;
	
#ifdef __BCC_LRC__

	for( i = 0 ; i < length ; i++)
	{
		bcc += buffer[i];
	}
	return bcc;

#elif defined __BCC_XOR__

	for( i = 0; i < length; i++)
	{
		bcc ^=buffer[i];
	}
	return bcc;

#endif
}		




void COM_txCMD_CHAN1(UINT8 deviceAddress, 
			UINT8 cmd, UINT8 *buffer , UINT8 length)
{
	UINT8 cmdPacket[25] = {0};
	UINT8 i,j,cs;

	i = 0;
	cmdPacket[i++]= CMD_SOP;
	cmdPacket[i++] = deviceAddress+2;
	cmdPacket[i++] = length;
	cmdPacket[i++] = cmd;
	for( j =0; j < length ; j++)
	{
		cmdPacket[i+j] = buffer[j];
	}
	i+= j;
 	cs = checksum(&cmdPacket[1], i - 1 );
	while((cs == CMD_SOP ) || (cs == CMD_EOP)) //if check sum matches sop or eop
	{
		cmdPacket[2]++; 						// change length
		cs = checksum(&cmdPacket[1], i - 1 ); //recalculate check sum
	}
	cmdPacket[i++] = cs;
	cmdPacket[i++] = CMD_EOP;

	for( j = 0 ; j < i ; j++)
	{
		UART1_write(cmdPacket[j]);
	}
	UART1_transmit();

}
	