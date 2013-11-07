#include <stdio.h>
#include "stm32f4xx.h"

#include "stm32f4xx_adc.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "led.h"

//extern int mode;

//void SysTick_Handler();
void swPwm(void);
