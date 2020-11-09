/*************************************************/
/* CANDL: Thread 1                               */
/* Description: Data is sent up to the server to */
/* be processed and saved over MQTT.             */
/* Originator: Asa McVay												 */
/* Contributors: Asa McVay  										 */
/*               Mohammed Zuaiter                */
/*************************************************/

#include "dataSend.h"
#include <string>
#include "jsmn.h"

void sendData(void){
	
	uint8_t* CANMessage;
	
	MQTTSetup();
	Subscribe(CMDtopic);
	
	while(1){
		while(getNextMsg(CANMessage) != HAL_ERROR){
			
		}
		
		
	}
	
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


//Setup MQTT with SSL enabled
uint8_t MQTTSSLSetup(){
	//setup messages to send
	char sslRCV[] = "AT+QMTCFG=“recv/mode”,0,0,1";
	char sslCFG[] = "AT+QMTCFG=“SSL”,0,1,2";
	char sslCA[] = "AT+QFUPL=“RAM:cacert.pem”,1758,100";
	char sslCC[] = "AT+QFUPL=“RAM:client.pem”,1220,100";
	char sslCK[] = "AT+QFUPL=“RAM:user_key.pem”,1679,100";
	char caCFG[] = "AT+QSSLCFG=“cacert”,2,“RAM:cacert.pem”";
	char ccCFG[] = "AT+QSSLCFG=“clientcert”,2,“RAM:client.pem”";
	char ckCFG[] = "AT+QSSLCFG=“clientkey”,2,“RAM:user_key.pem”";
	char sslMode[] = "AT+QSSLCFG=“seclevel”,2,2";
	char sslVersion[] = "AT+QSSLCFG=“sslversion”,2,4";
	char sslCipher[] = "AT+QSSLCFG=“ciphersuite”,2,0xFFFF";
	char sslTimeIgnore[] = "AT+QSSLCFG=“ignorelocaltime”,2,1";
	char startMQTTSSL[] = "AT+QMTOPEN=0,“68.119.81.176”,8883";
	char MQTTServerCon[] = "AT+QMTCONN=0,“M26_0206”";
	
	uint8_t rcvbuf[3];
	
	//send messages
	uartTransmit(sslRCV,rcvbuf);
	uartTransmit(sslCFG,rcvbuf);
	uartTransmit(sslCA,rcvbuf);
	uartTransmit(sslCC,rcvbuf);
	uartTransmit(sslCK,rcvbuf);
	uartTransmit(caCFG,rcvbuf);
	uartTransmit(ccCFG,rcvbuf);
	uartTransmit(ckCFG,rcvbuf);
	uartTransmit(sslMode,rcvbuf);
	uartTransmit(sslVersion,rcvbuf);
	uartTransmit(sslCipher,rcvbuf);
	uartTransmit(sslTimeIgnore,rcvbuf);
	uartTransmit(startMQTTSSL,rcvbuf);
	uartTransmit(MQTTServerCon,rcvbuf);
	
}


//Setup MQTT without SSL
uint8_t MQTTSetup(){
	//setup messages to send
	char RCV[] = "AT+QMTCFG=“recv/mode”,0,0,1";
	char startMQTT[] = "AT+QMTOPEN=0,“68.119.81.176”,1883";
	std::string MQTTServerConSTRING = "AT+QMTCONN=0,\"CANDLTest\"[,\"" + username + "\"[,\"" + password + "\"]]";
	char MQTTServerCon[MQTTServerConSTRING.length()+1];
	std::strcpy(MQTTServerCon,MQTTServerConSTRING.c_str());
	uint8_t rcvbuf[3];
	
	//send messages
	uartTransmit(RCV,rcvbuf);
	uartTransmit(startMQTT,rcvbuf);
	uartTransmit(MQTTServerCon,rcvbuf);
	
}


//Transmit uart message and check return value. Will always check against "OK"
//@param message: message to send to device
//@param rcvbuf: buffer to store answer back into
void uartTransmit(char message[]){
	uint8_t success[] = {'O','K', '\0'};
	uint8_t rcvbuf[3];
	
	//transmit message
	if (HAL_UART_Transmit(&huart2, (uint8_t*)message, sizeof(message), 100) != HAL_OK)
	{
		transmitErrorHandler(message);
	}
	HAL_Delay(500);
	
	//check received message. If unsuccessful or receive no "OK", go to error handler
	if (HAL_UART_Receive(&huart2,(uint8_t*)rcvbuf,3,10000) != HAL_OK)
	{
		receiveErrorHandler();
	}
	if (rcvbuf[0] != success[0] ||
		rcvbuf[1] != success[1] || rcvbuf[2] != success[2]){
			
		receiveErrorHandler();
	}
	
}


//Form subscribe message to send over UART
//@param topic: topic to subscribe to
void Subscribe(std::string topic){
	//form message
	std::string subMsgSTRING = "AT+QMTSUB=0,1,\""+topic+",2";
	char subMsg[subMsgSTRING.length() + 1];
	std::strcpy(subMsg,subMsgSTRING.c_str());
	
	//send subscribe message
	uartTransmit(subMsg);
}


//reads from the SD card and retrieves the next CAN message stored there
uint8_t getNextMsg(uint8_t* CANMessage){
	CANMessage = (uint8_t*) "test message";
	return HAL_OK;
}
