#include <stdio.h>
#include "stm32f4xx.h"

#include "stm32f4xx_adc.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"


float V25 = 0.76; // voltage reading at 25 degrees C
float LED_THRESHOLD = 2.0; // 2 degrees celsius to change LED

int PWM = 100000;							// Duty cycles per period

#define D	10 // filter depth
#define TRACKING_TEMPERATURE 0
#define DOING_PWM 1

static volatile uint_fast16_t ticks;
static float baseTemperature;

static int currentLED;


typedef struct  { // Struct to keep track of filter internals
	float buffer[D]; // buffer
	float sum; // average of values in the buffer
	float averageValue;
	short index; 				// Index of next buffer entry
	
} movingAverageFilter;

void SysTick_Handler();
void initializeLed();
void initializeAdc();
void initializeButton();
void initializeFilter(movingAverageFilter *filter);
void updateFilter(movingAverageFilter *filter, float temperature);
void updateLED(float temperature);
void pwn();
void trackTemperature();
void revertPinState();
int isButtonPressed();


static movingAverageFilter filter;
