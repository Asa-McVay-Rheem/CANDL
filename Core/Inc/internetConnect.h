//#include "main.h"
#ifndef intCon_seen
#define intCon_seen

#include "stm32l4xx_hal.h"
#include "dataSend.h"

void internetConnect(void);


extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

#endif
