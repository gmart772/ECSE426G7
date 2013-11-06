#include "led.h"

/**
 * @brief Initializes the LEDs for PWM (Output compare of TIM4). 
 * Inputs and outputs no variables.
 */void initLeds(void)
{	
  GPIO_InitTypeDef GPIO_InitStructure;

  /* GPIOD Clocks enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD , ENABLE);
  
  /* GPIOA Configuration: Channel 1 and 3 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_TIM3);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);
	
	configureLEDS(0, 0, 0, 0);

}

/**
 * @brief Configures the LEDs duty cycle with respect to TIM4 period of 65535.
 * A value of 65535 indicates a full duty cycle (LEDs forced on)
 * Outputs no variables.
 * @param LED1: New pulse time for LED1.
 * @param LED2: New pulse time for LED2.
 * @param LED3: New pulse time for LED3.
 * @param LED4: New pulse time for LED4.
 */
void configureLEDS(int LED1, int LED2, int LED3, int LED4)
{
	TIM4->CCR1 = LED1;
	TIM4->CCR2 = LED2;
	TIM4->CCR3 = LED3;
	TIM4->CCR4 = LED4;
}

/**
 * @brief Flashes the LEDs at a frequency dependent on the pitch and roll. 
 * Outputs no variables.
 * @param pitch: The pitch measured in degrees.
 * @param roll: The roll measured in degrees.
 */
void flashLeds(float pitch, float roll) {
	
	//float LED1, LED2, LED3, LED4;
	int32_t LED[4];
	
	if (roll > 3)
	{
		LED[0] = 0;
		LED[2] = ((TIM4_PERIOD)/90)*roll;
	}
	else if (roll < 3)
	{
		LED[0] = ((TIM4_PERIOD/90)*(abs(roll)));
		LED[2] = 0;
	}
	else
	{
		LED[0] = 0;
		LED[2] = 0;
	}
	
	if (pitch > 3)
	{
		LED[1] = 0;
		LED[3] = ((TIM4_PERIOD/90)*pitch);
	}
	else if (pitch < 3)
	{
		LED[1] = ((TIM4_PERIOD/90)*(abs(pitch)));
		LED[3] = 0;
	}
	else
	{
		LED[1] = 0;
		LED[3] = 0;
	}

	// Program the new values into the LEDs pulse time
	configureLEDS(LED[0], LED[1], LED[2], LED[3]);	
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
