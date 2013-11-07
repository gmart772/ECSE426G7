#include <stdio.h>
#include "stm32f4xx.h"

#include "stm32f4xx_adc.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "tempFilter.h"
#include "cmsis_os.h"
#include "led.h"

static volatile uint_fast16_t ticks;
static float baseTemperature;
static int currentLED;
static temperatureFilter filter;

static float V25 = 0.76f; // voltage reading at 25 degrees C
static float LED_THRESHOLD = 2.0; // 2 degrees celsius to change LED

void trackTemperature(void);
void revertPinState(void);
void initializeLed(void);
void initializeAdc(void);
void initializeButton(void);
void updateLED(float temperature);
int isButtonPressed(void);

