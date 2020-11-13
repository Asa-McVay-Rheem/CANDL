/*************************************************/
/* CANDL: Thread 2                               */
/* Description: CAN and GPS data are saved and   */
/* stored on a micro sd card. 									 */
/* Originator: Asa McVay												 */
/* Contributors: Jack Burrows										 */
/*************************************************/

#include "dataCollect.h"
//#include "main.h"
//#include <string.h>
//#include <assert.h>
//#include <libopencm3/stm32/rcc.h>
//#include <libopencm3/stm32/gpio.h>
//#include <libopencm3/stm32/can.h>

// function called from StartDataCollect function in main
void storeData(void){
	while(1);
	
/*CAN_TxHeaderTypeDef msg;

msg.StdId = 127; // 11 bit ID
msg.ExtId = 1; // 29 bit ID
msg.IDE = CAN_ID_STD; // choose between 11 or 29 bit ID
msg.RTR = CAN_RTR_DATA; // signifies a data frame
msg.DLC = 8; // length of message
msg.TransmitGlobalTime = DISABLE;

uint8_t txData[8];
uint32_t mb; // this doesn't matter too much

if (HAL_CAN_AddTxMessage(&hcan, &msg, txData, &mb) != HAL_OK) {
    Error_Handler();
}*/
	
}



