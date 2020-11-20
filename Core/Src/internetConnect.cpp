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

//make a connection to the internet over wifi or LTE
void internetConnect(void){
	//Disable Echo, setup terminating char, and change response codes
	uartSetup();
	
	sendData();
	//while(1);
	
}

void uartSetup() {
	char ate[] = "ATE0\r";	//Echo disable
	uartTransmit(ate, 5);
	commandtermination = '\0';
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
	if(mode != 0)		//return false if setup failed
		return false;
	
	mode = cmode;
	buff = buffin;
	buffi = 0;
	bufflength = len;
	
	return true;
}

int receiveComplete() {
	while(mode != 0 || mode != 1)	//Wait for completion
		osDelay(10);
	
	if (mode == 1)
		return -1;
	
	return buffi;
	
}

