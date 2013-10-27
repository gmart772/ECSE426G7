#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

#define NO_TIMEOUT 0
#define TIMEOUT_OCCURRED 1

extern short interruptStatus;

void initTimer(void);
void TIM3_IRQHandler(void);


