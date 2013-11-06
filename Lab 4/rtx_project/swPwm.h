#include <stdio.h>
#include "stm32f4xx.h"

#include "stm32f4xx_adc.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "temperatureTracking.h"

static int current_cycle_length = 0;
static int pwm_counter = 0;

//void SysTick_Handler();
void swPwm(void);
