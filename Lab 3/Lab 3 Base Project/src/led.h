#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

/*	--------------------------------------------		*/
// 									GPIO_PIN_13          						//
//		GPIO_PIN_12   								GPIO_PIN_14   	//
//									GPIO_PIN_15   									//
/*	---------------------------------------------		*/

static volatile uint_fast16_t ticks;

static int frequencyDivisions;

void initLeds(void);
void initLedsForPWM(void);
void flashLeds(float pitch, float roll);
void SysTick_Handler(void);
float abs(float f);
