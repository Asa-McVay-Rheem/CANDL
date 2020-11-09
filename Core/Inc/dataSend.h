//#include "MQTTClient.h"
//#include "main.h"
#ifndef datSen_Seen
#define datSen_Seen

#include "internetConnect.h"
#include <string>
void sendData(void);
void transmitErrorHandler(char message[]);
void receiveErrorHandler(void);
void uartTransmit(char message[], uint8_t rcvbuf[]);
uint8_t MQTTSetup(void);
uint8_t MQTTSSLSetup(void);
void Subscribe(std::string topic);
uint8_t getNextMsg(uint8_t* CANMessage);

const std::string CANtopic = "CAN";
const std::string GPStopic = "GPS";
const std::string CMDtopic = "Command";
const std::string ERRtopic = "Error";

const std::string username = "test";
const std::string password = "password";

#endif
