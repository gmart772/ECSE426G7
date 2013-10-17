#include <stdio.h>
#include "stm32f4xx.h"

#include "stm32f4xx_adc.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

float V25 = 0.76; // voltage reading at 25 degrees C
float LED_THRESHOLD = 2.0; // 2 degrees celsius to change LED

int PWM = 100;							// Duty cycles per period

#define D	5 // filter depth

static volatile uint_fast16_t ticks;

typedef struct  { // Struct to keep track of filter internals
	float buffer[D]; // buffer
	float sum; // average of values in the buffer
	float averageValue;
	short index; 				// Index of next buffer entry
} movingAverageFilter;

void SysTick_Handler();
void initializeLed();
void initializeAdc();
void initializeFilter(movingAverageFilter *filter);
void updateFilter(movingAverageFilter *filter, float temperature);
void updateLED(float temperature, short *LED);
void pwn();
