//header file for the thread where CAN and GPS data are stored

#ifndef __storedat
#define __storedat

#include "stm32l4xx_hal.h"

void storeData(void);

extern char SD_Path[];
extern char CANdataFile[];

extern char GPSdataFile[];
extern char configFile[];

extern CAN_HandleTypeDef hcan1;

extern CAN_TxHeaderTypeDef pHeader; //declare a specific header for message transmittions
extern CAN_RxHeaderTypeDef pRxHeader; //declare header for message reception
extern uint32_t TxMailbox;
extern uint8_t a,r; //declare byte to be transmitted //declare a receive byte
extern CAN_FilterTypeDef sFilterConfig; //declare CAN filter structure

extern int receivedCAN;

#endif
