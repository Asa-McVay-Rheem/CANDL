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
	sendData();
	while(1);
	
}

<<<<<<< Updated upstream
=======

bool uartMainTransmitAT(char message[], uint8_t len) {
	char msgEnd[] = "\r\nOK\r\n";	//Lines end in carrage return \r then linefeed \n
	uint8_t rcvbuf[1];
	len = len+20;
	char msgbuf[len];
	
	//transmit message
	message[len - 1] = '\r';	//Command line termination character not null but cr \r
	if (HAL_UART_Transmit(&huart2, (uint8_t*)message, len, 100) != HAL_OK)
	{
		transmitErrorHandler(message);
	}
	//HAL_Delay(500);
	
	//check received message for OK. If unsuccessful to recieve message go to error handler
	uint8_t i = 0;
	while(i < len+20){	//OK is prefixed with \r\n confirmed in puttty log and notepad++
		if (HAL_UART_Receive(&huart2,rcvbuf,1,10000) != HAL_OK)
		{
			receiveErrorHandler();
		}
		msgbuf[i] = rcvbuf[0];
		if((i >= 6) && (msgbuf[i-6] == msgEnd[0] 
		&& msgbuf[i-5] == msgEnd[1] && msgbuf[i-4] == msgEnd[2]
		&& msgbuf[i-3] == msgEnd[3] && msgbuf[i-2] == msgEnd[4]
		&& msgbuf[i-1] == msgEnd[5] && msgbuf[i]   == msgEnd[6])){
			return true;	//Found OK
		}
		i++;
	}

	return false;	//OK not found and expired msg buff
}
>>>>>>> Stashed changes
