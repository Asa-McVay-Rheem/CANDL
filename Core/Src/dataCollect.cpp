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

CAN_RxHeaderTypeDef pRxHeader; //declare header for message reception
uint32_t TxMailbox;
uint8_t r; //declare a receive byte
CAN_FilterTypeDef sFilterConfig; //declare CAN filter structure
//CANMessage = msg;

int receivedCAN = 0;

// function called from StartDataCollect function in main
void storeData(void){
	//while(1);
	
  //pHeader.DLC=1; //give message size of 1 byte
	//pHeader.IDE=CAN_ID_STD; //set identifier to standard
	//pHeader.RTR=CAN_RTR_DATA; //set data type to remote transmission request?
	//pHeader.StdId=0x244; //define a standard identifier, used for message identification by filters
	
	//filter one (stack light blink)
	//sFilterConfig.FilterFIFOAssignment=CAN_FILTER_FIFO0; //set fifo assignment
	//sFilterConfig.FilterIdHigh=0; //the ID that the filter looks for
	//sFilterConfig.FilterIdLow=0;
	//sFilterConfig.FilterMaskIdHigh=0xFFFF;
	//sFilterConfig.FilterMaskIdLow=0xFFFF;
	//sFilterConfig.FilterScale=CAN_FILTERSCALE_32BIT; //set filter scale
	sFilterConfig.FilterActivation=CAN_FILTER_DISABLE;
	
	//HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig); //configure CAN filter

	if (HAL_CAN_Start(&hcan1) != HAL_OK){
		while(1);
	} //start CAN
	if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK){
		while(1);
	} //enable interrupts
	if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK){
		while(1);
	} //enable interrupts
	
	while(receivedCAN == 0);
	receivedCAN = 0;
	
	while(1);
	
	///////////////////////////////// 
	if(f_mount(&SDFatFs, (TCHAR const*)SD_Path, 0) != FR_OK){
		while(1);
 	} 
	else {
	// Create a FAT file system (format) on the logical drive
 	// WARNING: Formatting the uSD card will delete all content on the device 
		if(f_mkfs((TCHAR const*)SD_Path, 0, 0) != FR_OK){
	 while(1);
 	} 
	else {
	// Create & Open a new text file object with write access
 	if(f_open(&MyFile, "CANDL.txt", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK){
	while(1);
 	} 
	else {
 	// Write data to the text file 
 	res = f_write(&MyFile, pRxHeader, sizeof(msg), (void*)&byteswritten);
 	if((byteswritten == 0) || (res != FR_OK)){
 	// 'CANDL.txt' file Write or EOF Error 
 	
 	while(1);
 	} 
	else {
 	// Successful open/write
 	
	f_close(&MyFile);
 	/*// Open the text file object with read access 
 	if(f_open(&MyFile, "CANDL.txt", FA_READ) != FR_OK){
 	// 'CANDL.txt' file Open for read Error
 	
 	while(1);
 	} 
	else {
 	// Read data from the text file 
 	res = f_read(&MyFile, rtext, sizeof(wtext), &r);
		if((strcmp(rtext,wtext)!=0)|| (res != FR_OK)){
 	// 'CANDL.txt' file Read or EOF Error 	
 	while(1);
 	} 
	else {
 	// Successful read
 	// Close the open text file 
 	f_close(&MyFile);
	
		}*/
	
	
	
}



