/************************************/
/* Contributors: Asa McVay					*/
/* Description: Create glue between */
/* MQTT and the HAL functions		    */
/************************************/

#include "MQTTClient.h"
#include "TimerGlue.h"

void TimerInit(Timer*);
char TimerIsExpired(Timer*);
void TimerCountdownMS(Timer*, unsigned int);
void TimerCountdown(Timer*, unsigned int);
int TimerLeftMS(Timer*);