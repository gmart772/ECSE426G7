#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

#define NO_TIMEOUT 0
#define TIMEOUT_OCCURRED 1

#define ACCELEROMETER 0
#define PWM 1

extern short timerInterrupt;

void initTimer(short mode);
void TIM3_IRQHandler(void);


