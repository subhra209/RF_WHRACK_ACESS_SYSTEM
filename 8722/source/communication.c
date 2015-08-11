
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
	UINT8 i;

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



#ifdef __LOOP_BACK__
void COM1_task()
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
	if( UART1_hasData() == TRUE )
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
	if( UART2_hasData() == TRUE)
	{
		uartData = UART2_read();
		UART2_write(uartData);
	//	UART2_transmit();
	}
#endif
		return;



}

#else

void COM1_task(void)
{
	volatile UINT8 uartData = 0,i;
	communication[0].curAppTime = GetAppTime();
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

for(i = 0 ; i < ACTIVE_USARTS ; i++ )
{
	if( communication[i].prevAppTime != communication[i].curAppTime)
	{
		if( communication[i].prevState == communication[i].state && (communication[i].state == COM_IN_PACKET_COLLECTION))
		{
			--communication[i].timeout ;
			if( communication[i].timeout == 0)
			{
				COM1_restart();
				return;
			}
			
		}
		
		communication[i].prevAppTime = communication[i].curAppTime;
	}

	switch( communication[i].state)
	{
		case COM_START:
		if( i == UART1)
		{
			if( UART1_hasData() == FALSE )
					return;
			
				uartData = UART1_read();
			#ifdef	PASS_THROUGH
				UART2_write(uartData);
				UART2_transmit();
			#endif	

		}
		else if( i == UART2)
		{
			if( UART2_hasData() == FALSE )
				return;

			uartData = UART2_read();	

		}
			
		if( uartData == communication[i].rx_sop )
		{
			communication[i].rxPacketIndex = 0;
			communication[i].state = COM_IN_PACKET_COLLECTION;
		}
		break;

		case COM_IN_PACKET_COLLECTION:

			if( i == UART1)
			{
				if( UART1_hasData() == FALSE )
						return;
				
					uartData = UART1_read();
				#ifdef	PASS_THROUGH
					UART2_write(uartData);
					UART2_transmit();
				#endif	
	
			}
			else if( i == UART2)
			{
				if( UART2_hasData() == FALSE )
					return;
	
				uartData = UART2_read();	
	
			}

			communication[i].rxPacketBuffer[communication[i].rxPacketIndex++]=uartData;
			communication[i].timeout = 0;
			if( communication[i].rxPacketIndex >= RX_PACKET_SIZE)
			{
				communication[i].txPacketBuffer[COM_DEVICE_ADDRESS_INDEX] = DEVICE_ADDRESS;	//store device address
				++communication[i].txPacketLength;

				communication[i].txPacketBuffer[COM_TX_CODE_INDEX] = COM_RESP_OVERRUN;		//store tx code
				++communication[i].txPacketLength;
				
				communication[i].state = COM_IN_TX_DATA;
				
			}
		
			if(communication[i].rxPacketIndex >= PACKET_LENGTH )
			{
				UINT8 *txData ;
				UINT8 parseResult = 0;
				COM_RESP_CODE txCode = COM_RESP_NONE;

				communication[i].rxPacketBuffer[communication[i].rxPacketIndex++] = '\0';


#ifdef __NO_CHECKSUM__
				parseResult = PARSE_SUCCESS;
#else				
				parseResult = parse1Packet((&txCode), i);		//parse packet 
#endif

				switch( parseResult)
				{
					case IGNORE:
					COM1_reset();	
					return;
					
					case PARSE_SUCCESS:
											
					if( communication[i].callBack != 0 )
					{
						communication[i].state = COM_START;
						communication[i].txPacketLength = 	communication[i].callBack(&communication[i].rxPacketBuffer[COM_RX_DATA_START_INDEX], 
																&communication[i].txCode,&txData);
																							  
						communication[i].txPacketBuffer[COM_DEVICE_ADDRESS_INDEX] = DEVICE_ADDRESS;	//store device address
						++communication[i].txPacketLength;

						communication[i].txPacketBuffer[COM_TX_CODE_INDEX] = communication[i].txCode;	//store tx code
						++communication[i].txPacketLength;

						for( i = COM_TX_DATA_START_INDEX ; i < communication[i].txPacketLength ; i++)	//store data
						{
							communication[i].txPacketBuffer[i] = *txData;
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

						communication[i].txPacketBuffer[COM_DEVICE_ADDRESS_INDEX] = DEVICE_ADDRESS;	//store device address
						++communication[i].txPacketLength;
						
						communication[i].txPacketBuffer[COM_TX_CODE_INDEX] = txCode;		//store tx code
						++communication[i].txPacketLength;
						
					}
					
					break;
					
					default:
					break;
					}
					communication[i].state = COM_IN_TX_DATA;
				}

			
				break;
	
			case COM_IN_TX_DATA:
	
				COM_txData();
	
				COM1_reset();
		
			break;
	
			default:
				COM1_reset();
			break;

		}
		communication[i].prevState = communication[i].state;

	}
}


#endif

UINT8 parse1Packet(UINT8 *respCode , UINT8 uart)
{

	UINT8 receivedChecksum = communication[uart].rxPacketBuffer[communication[uart].rxPacketIndex-1];
	UINT8 genChecksum = 0;


	if((communication[uart].rxPacketBuffer[ COM_DEVICE_ADDRESS_INDEX] != DEVICE_ADDRESS)
			&& (communication[uart].rxPacketBuffer[COM_DEVICE_ADDRESS_INDEX] != BROADCAST_ADDRESS) )
		return IGNORE;
	
	genChecksum = checksum(communication[uart].rxPacketBuffer,communication[uart].rxPacketIndex-1);
	
	if( receivedChecksum == genChecksum)
	{
		--communication[uart].rxPacketIndex;
		communication[uart].rxPacketBuffer[communication[uart].rxPacketIndex] = '\0'; //remove checksum from packet
	 
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
	