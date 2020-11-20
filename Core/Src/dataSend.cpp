/*************************************************/
/* CANDL: Thread 1                               */
/* Description: Data is sent up to the server to */
/* be processed and saved over MQTT.             */
/* Originator: Asa McVay												 */
/* Contributors: Asa McVay  										 */
/*               Mohammed Zuaiter                */
/*************************************************/

#include "dataSend.h"

std::string UID;
bool pubTime;

void sendData(void){
	
	//timer setup
	//TimerHandle_t pubTim = xTimerCreate("Publish Data Timer",pdMS_TO_TICKS(ONEHOUR),pdTRUE,(void *) 0,pubTimCallback);
	//xTimerStart(pubTim, 1000);
	pubTime = true;
	
	//buffers to store data
	uint8_t CANMessage[8];
	uint8_t rcvMsgBuf[20];
	
	//obtain unique ID in string form
	UID = to_string(HAL_GetUIDw0())+to_string(HAL_GetUIDw1())+to_string(HAL_GetUIDw2());
	
	MQTTSetup();
	Subscribe(CMDtopic);
	
	while(1){
		if(pubTime == true){
			//pubTime = false;
			if (getNextMsg(CANMessage) != HAL_ERROR){
				do{
						Publish(CANMessage,64,CANtopic);
				}while(getNextMsg(CANMessage) != HAL_ERROR);
			}
			else{
				char errMsg[] = "Error: No available messages to send";
				Publish((uint8_t*)errMsg,sizeof(errMsg),ERRtopic);
			}
		}
		if (messageReceived(rcvMsgBuf)){
			//parse message and handle command TBD
		}
	}
}



//Setup MQTT with SSL enabled
uint8_t MQTTSSLSetup(){
	//setup messages to send
	char sslRCV[] = "AT+QMTCFG=\"recv/mode\",0,0,1";
	char sslCFG[] = "AT+QMTCFG=\"SSL\",0,1,2";
	char sslCA[] = "AT+QFUPL=\"RAM:cacert.pem\",1758,100";
	char sslCC[] = "AT+QFUPL=\"RAM:client.pem\",1220,100";
	char sslCK[] = "AT+QFUPL=\"RAM:user_key.pem\",1679,100";
	char caCFG[] = "AT+QSSLCFG=\"cacert\",2,\"RAM:cacert.pem\"";
	char ccCFG[] = "AT+QSSLCFG=\"clientcert\",2,\"RAM:client.pem\"";
	char ckCFG[] = "AT+QSSLCFG=\"clientkey\",2,\"RAM:user_key.pem\"";
	char sslMode[] = "AT+QSSLCFG=\"seclevel\",2,2";
	char sslVersion[] = "AT+QSSLCFG=\"sslversion\",2,4";
	char sslCipher[] = "AT+QSSLCFG=\"ciphersuite\",2,0xFFFF";
	char sslTimeIgnore[] = "AT+QSSLCFG=\"ignorelocaltime\",2,1";
	char startMQTTSSL[] = "AT+QMTOPEN=0,\"68.119.81.176\",8883";
	char MQTTServerCon[] = "AT+QMTCONN=0,\"M26_0206\"";
	
	//send messages
	uartTransmit(sslRCV, sizeof(sslRCV));
	uartTransmit(sslCFG, sizeof(sslCFG));
	uartTransmit(sslCA, sizeof(sslCA));
	uartTransmit(sslCC, sizeof(sslCC));
	uartTransmit(sslCK, sizeof(sslCK));
	uartTransmit(caCFG, sizeof(caCFG));
	uartTransmit(ccCFG, sizeof(ccCFG));
	uartTransmit(ckCFG, sizeof(ckCFG));
	uartTransmit(sslMode, sizeof(sslMode));
	uartTransmit(sslVersion, sizeof(sslVersion));
	uartTransmit(sslCipher, sizeof(sslCipher));
	uartTransmit(sslTimeIgnore, sizeof(sslTimeIgnore));
	uartTransmit(startMQTTSSL, sizeof(startMQTTSSL));
	uartTransmit(MQTTServerCon, sizeof(MQTTServerCon));
	
}


//Setup MQTT without SSL
uint8_t MQTTSetup(){
	//setup messages to send
	char RCV[] = "AT+QMTCFG=\"recv/mode\",0,0,1";
	char startMQTT[] = "AT+QMTOPEN=0,\"68.119.81.176\",1883";
	std::string MQTTServerConSTRING = "AT+QMTCONN=0,\"CANDLTest\""; //[,\"" + username + "\"[,\"" + password + "\"]]";
	/// [ in example represent optional values
	uint8_t len = MQTTServerConSTRING.length();
	
	//send messages
	uartTransmit(RCV, 28);
	uartTransmit(startMQTT, 34);
	uartTransmit(string2char(MQTTServerConSTRING), len);
	
}


//Transmit uart message and check return value. Will always check against "OK"
//@param message: message to send to device
//@param rcvbuf: buffer to store answer back into


/*void uartTransmit(char message[], uint8_t len){
	uint8_t success[] = {'\r','\n','O','K', '\r', '\n'};	//Terminal would output
	uint8_t rcvbuf[6];
	
	//transmit message
	message[len - 1] = '\r';	//Command line termination character not null but cr \r
	if (HAL_UART_Transmit(&huart2, (uint8_t*)message, len, 100) != HAL_OK)
	{
		transmitErrorHandler(message);
	}
	//HAL_Delay(500);
	
	//check received message. If unsuccessful or receive no "OK", go to error handler
	if (HAL_UART_Receive(&huart2,(uint8_t*)rcvbuf,6,10000) != HAL_OK)
	{
		receiveErrorHandler();
	}
	if (rcvbuf[0] != success[0] || rcvbuf[1] != success[1] || rcvbuf[2] != success[2] || rcvbuf[3] != success[3] || rcvbuf[4] != success[4]
		 || rcvbuf[5] != success[5]){
			
		receiveErrorHandler();
	}
	
}*/


//Form subscribe message to send over UART
//@param topic: topic to subscribe to
void Subscribe(std::string topic){
	//form message
	std::string subMsg = "AT+QMTSUB=0,1,\""+topic+",2";
	
	//send subscribe message
	uartTransmit(string2char(subMsg), subMsg.length());
}


void Publish(uint8_t* Message, uint8_t len, std::string topic){
	char rcvbuf[10];										//buffer returned message is written to
	
	//convert Message to string
	std::string newmsg;
	newmsg.reserve(len);
	for(int i=0; i<len;i++){
		newmsg += Message[i];
	}
	//form AT command and JSON message
	std::string ATmsg = "AT+QMTPUBEX=0,1000,1,0,"+topic+","+to_string(newmsg.length());	//Why was there a +1
	std::string toJSON = "{\"UID:\""+UID+",\"Payload\":"+newmsg+"}";
	
	//Setup recieve
	receiveSetup(rcvbuf, 10, 'm');
	//transmit AT message
	if (HAL_UART_Transmit(&huart2, (uint8_t*)string2char(ATmsg), sizeof(ATmsg), 100) != HAL_OK)
	{
		transmitErrorHandler(string2char(ATmsg));
	}
	//HAL_Delay(500);
	int i = receiveComplete(); //Wait for finish, get length of buff
	//check received message. If unsuccessful or receive no ">", go to error handler
	if (i < 3)
	{
		receiveErrorHandler();
	}
	if (rcvbuf[i-3] != '>'){	//Get 3rd to last char
		receiveErrorHandler();
	}
	
	//send JSON message to publish
	uartTransmit(string2char(toJSON), toJSON.length());
}



//reads from the SD card and retrieves the next CAN message stored there
//@param CANMessage: array to store message to
//@return: success or fail
uint8_t getNextMsg(uint8_t* CANMessage){
	CANMessage = (uint8_t*) "test message";
	return HAL_OK;
}


bool messageReceived(char * msgbuf){
	char ATbufcheck[] = "AT+QMTRECV=0,0";	//Consider deleting ,0
	char msgEnd = '4';
	char msgOK = '0';
	
	uint8_t i = uartTransmit(ATbufcheck, sizeof(ATbufcheck), msgbuf, 20);
	
	if(msgbuf[i-2] == msgOK) {
		return true;
	}
	
	else if(msgbuf[i-2] == msgEnd) {
		return false;
	}
	//Warning other error
	return false;
}


//convert int to a string
std::string to_string(uint32_t num){
	std::ostringstream ostr;
	ostr << num;
	
	std::string numString = ostr.str();
	
	return numString;
}


//convert string to a char array
char* string2char(std::string str){
	char* result;
	for (int i = 0; i <= sizeof(str); i++) {
        result[i] = str[i];
    }
	return result;
}

void pubTimCallback(TimerHandle_t xTimer){
	pubTime = true;
}


//Error handler for if transmission fails
//@param message: message attempted to send
void transmitErrorHandler(char message[]){
	while(1);
	
}


//Error handler for if receive fails
void receiveErrorHandler(void){
	while(1);
	
}

