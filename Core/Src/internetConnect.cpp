/*************************************************/
/* CANDL: Thread 1                               */
/* Description: A connection to the internet is  */
/* made over WiFi or LTE. Provisioning mode is   */
/* also handled in this file.                    */
/* Originator: Asa McVay												 */
/* Contributors: Mohammed Zuaiter                */
/*               Asa McVay											 */
/*************************************************/

#include "internetConnect.h"
//#include "stm32l4xx_it.h"
	char uart2commandtermination;	//line termination char
	char * uart2buff;						//Pointer to buffer
	int uart2bufflength;		//Initial buffer size
	int uart2buffi;				//Return buffer size
	char uart2mode;	//Specifiy Mode of operation


/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */
	if (USART2->ISR & USART_ISR_ORE) // Overrun Error
		USART2->ICR = USART_ICR_ORECF;
	if (USART2->ISR & USART_ISR_NE) // Noise Error
		USART2->ICR = USART_ICR_NCF;
	if (USART2->ISR & USART_ISR_FE) // Framing Error
		USART2->ICR = USART_ICR_FECF;
	if (USART2->ISR & USART_ISR_RXNE) // Received character?
	{
		USART2->ISR &= ~USART_ISR_RXNE;
		char rx = (char)(USART2->RDR & 0x00FF);	//Recieved Char
		int i = 0;
		if((uart2buffi >= uart2bufflength))
					uart2mode = 1; //Error
		else
		switch(uart2mode) {
			
			case 'c' :	//Code search for termination char
				uart2buff[uart2buffi] = rx;
				uart2buffi++;
				if(uart2buffi > 1)
					if(rx == uart2commandtermination)	//Check only for termination
						uart2mode = 0;	//Done
			break;
			
			case 'm' :	//Look for message only
			case 'n' :	//Look for message and return OK
				uart2buff[uart2buffi] = rx;
				uart2buffi++;
				if(uart2buffi > 2)
					if(rx == '\n' && uart2buff[uart2buffi-2] == uart2commandtermination) {	//Check for termination and linefeed
						if(uart2mode == 'm')
							uart2mode = 0;	//Done
						else if (uart2mode == 'n')
							uart2mode = 'c';
					}					
			break;
					
			case 'r' :	//Look for start of length bytes
				if(rx == ' ') {
					uart2buffi = 0;
					uart2mode = 'x';
				}
			break;
			
			case 'x' :	//Perform math
				switch (rx){
					case '9' : i++;
					case '8' : i++;
					case '7' : i++;
					case '6' : i++;
					case '5' : i++;
					case '4' : i++;
					case '3' : i++;
					case '2' : i++;
					case '1' : i++;
					case '0':	//Move one digit and add the rest
						uart2buffi = uart2buffi * 10 + i;
					break;
					
					default:	//Line termination
						uart2mode = 'y';
					break;
				}
			break;
				
			case 'y':
				if(rx == '\n')	uart2mode = 'z';
				else		uart2mode = 1;
				
				if(uart2buffi == 0)
					uart2mode = 0;
				
				if (uart2buffi < uart2bufflength)
					uart2bufflength = uart2buffi;			//Load max into uart2bufflength
				uart2buffi = 0;
			break;
			
			case 'z':
				uart2buff[uart2buffi] = rx;
				uart2buffi++;
				if(uart2buffi == uart2bufflength)	//When reach max, return to mode 0
					uart2mode = 0;
			break;
		}
	}
	/* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

//make a connection to the internet over wifi or LTE
void internetConnect(void){
	//Disable Echo, setup terminating char, and change response codes
	uartSetup();
	
	sendData();
	//while(1);
	
}

void uartSetup() {
	uart2commandtermination = '\r';
	USART2->CR1 |= USART_CR1_RXNEIE;	//Test Enable
	uart2mode = 1;
	char ate[] = "ATE0\r";	//Echo disable
	uartTransmit(ate, 5);
	uart2commandtermination = '\0';
	char atv[] = "ATS3=0\r";
	uartTransmit(atv, 7);
	char coder[] = "ATV0";
	uartTransmit(coder, 5);	//Enable Response codes instead
}

//Break or stop after receiving \r or \0 depending on terminating char
int uartTransmit(char message[], uint8_t len, char* buffer, int bufflen) {
	if(!receiveSetup(buffer, bufflen, 'c'))
		receiveErrorHandler();
	
	if (HAL_UART_Transmit(&huart2, (uint8_t*)message, len, 100) != HAL_OK)
	{
		transmitErrorHandler(message);
	}
	
	return receiveComplete();
}

bool uartTransmit(char message[], uint8_t len) {
	char msgbuf[30];
	
	if(uartTransmit(message, len, msgbuf, 30) < 0)
		return false;
	
	return true;
}

bool receiveSetup(char* buffin, uint8_t len, char cmode) {
	if(!(uart2mode == 0x00 || uart2mode == 0x01))		//return false if setup failed
		return false;
	
	uart2mode = cmode;
	uart2buff = buffin;
	uart2buffi = 0;
	uart2bufflength = len;
	
	return true;
}

int receiveComplete() {
	while(!(uart2mode == 0x00 || uart2mode == 0x01))	//Wait for completion
		osDelay(10);
	
	if (uart2mode == 1)
		return -1;
	
	return uart2buffi;
	
}

