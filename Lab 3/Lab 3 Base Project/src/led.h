#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

/*	--------------------------------------------		*/
// 									GPIO_PIN_13          						//
//		GPIO_PIN_12   								GPIO_PIN_14   	//
//									GPIO_PIN_15   									//
/*	---------------------------------------------		*/

#define TIM4_PERIOD 65535

static volatile uint_fast16_t ticks;

static int frequencyDivisions;

void initLeds(void);

void flashLeds(float pitch, float roll);

float abs(float f);
