//#include "main.h"
#ifndef intCon_seen
#define intCon_seen

#include "stm32l4xx_hal.h"
#include "dataSend.h"

void internetConnect(void);

bool uartTransmit(char message[], uint8_t len);
int uartTransmit(char message[], uint8_t len, char* buffer, int bufflen);

void uartSetup();

bool receiveSetup(char* buffin, uint8_t len, char cmode);
int receiveComplete();

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

	extern char commandtermination;
	extern char* buff;
	extern int bufflength;
	extern int buffi;
	extern char mode;

#endif
