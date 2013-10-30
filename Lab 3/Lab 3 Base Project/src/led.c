#include "led.h"

/**
 * @brief Initializes the LEDs. 
 * Inputs and outputs no variables.
 */void initLeds(void) {
	
	// Initialize LED GPIOA
	GPIO_InitTypeDef gpio_init_s;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_StructInit(&gpio_init_s);
	gpio_init_s.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	gpio_init_s.GPIO_Mode = GPIO_Mode_OUT;
	gpio_init_s.GPIO_Speed = GPIO_Speed_100MHz;
	gpio_init_s.GPIO_OType = GPIO_OType_PP;
	gpio_init_s.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &gpio_init_s);

	ticks = 0;
	// Configured for 50 ms period
	// Configure SysTick to be 20Hz
	 pulseFreq = 1;
	SysTick_Config(SystemCoreClock / 4);
}

/**
 * @brief Flashes the LEDs at a frequency dependent on the pitch and roll. 
 * Outputs no variables.
 * @param pitch: The pitch measured in degrees.
 * @param roll: The roll measured in degrees.
 */
void flashLeds(float pitch, float roll) {
	int frequencyPitch;
	
	if ((abs(pitch) >= 60) && (abs(pitch) <= 90)) {
		frequencyPitch = 15;
	}
	else if ((abs(pitch) >= 30 && abs(pitch) <= 60)) {
		frequencyPitch = 7;
	}
	else if (abs(pitch) < 30 /* && abs(pitch) >= 15 */) {
		frequencyPitch = 2;
	}
	else /* if (abs(pitch) < 15) */ {
		frequencyPitch = 2;
	}
		
	int frequencyRoll;
	
	if ((abs(roll) <= 90) && (abs(roll) > 60)) {
		frequencyRoll = 15;
	}
	else if ((abs(roll) >= 30) && (abs(roll) <= 60)) {
		frequencyRoll = 7;
	}
	else if (abs(roll) < 30 /* && (abs(roll) >= 15) */) {
		frequencyRoll = 2;
	}
		else /*if (abs(roll) < 15) */ {
		frequencyRoll = 2;
	}
	
	if (pulseFreq != (frequencyRoll * frequencyPitch)) {
		pulseFreq = frequencyRoll * frequencyPitch;
	}
	
	SysTick_Config(SystemCoreClock / pulseFreq);
	
	// Wait for interrupt
	while(!ticks);
	// Set ticks to 0
	ticks = 0;
	GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
	GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
	GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
	GPIO_ToggleBits(GPIOD, GPIO_Pin_14);

}

void SysTick_Handler(void) {
	ticks = 1;
}

/**
 * @brief Get the absolute value of a float. 
 * @param f: Float value.
 * @retval The absolute value of f.
 */
float abs(float f) {
	if (f < 0) {
		return -f;
	}
	return f;
}
