//#include "MQTTClient.h"
//#include "main.h"
#ifndef datSen_Seen
#define datSen_Seen

#include "internetConnect.h"
#include <string>
//#include "jsmn.h"
#include <iostream>
#include <sstream>
#include "cmsis_os.h"

#define ONEHOUR				3600000

void sendData(void);

void transmitErrorHandler(char message[]);
void receiveErrorHandler(void);
void uartTransmit(char message[], uint8_t len);
uint8_t MQTTSetup(void);
uint8_t MQTTSSLSetup(void);
void Subscribe(std::string topic);
uint8_t getNextMsg(uint8_t* CANMessage);
void Publish(uint8_t* Message, uint8_t len, std::string topic);
std::string to_string(uint32_t num);
char* string2char(std::string str);
void pubTimCallback(TimerHandle_t xTimer);
bool messageReceived(uint8_t* msgbuf);

const std::string CANtopic = "CAN";
const std::string GPStopic = "GPS";
const std::string CMDtopic = "Command";
const std::string ERRtopic = "Error";

const std::string username = "test";
const std::string password = "password";

extern std::string UID;

#endif
