#include "main.h"

/* Main function. Initializes LED and ADC. Begins taking voltage measurements and converts them to temperatures.
Applies filter to values. */
int main()
{
	initializeLed();
	initializeAdc();
	
	//uint16_t voltage;
	float voltage;
	float temperature;
	float avgSlope = 2.5; // value from datasheet
	movingAverageFilter *filter;
	
	// pointer to currently used LED index (used to cycle LEDs)
	short *currentLED;
	currentLED[0] = 0;
	
	int current_cycle_length = 0;
	int pwm_counter = 0;
	int pwm_on = 1;
	
	ticks = 0;
	// Configured for 50 ms period
	// Configure SysTick to be 20Hz
	// NOTE: argument here must be less than 0xFFFFFF; //(24 bit timer)
	SysTick_Config(SystemCoreClock / 20); // Number of ticks between two interrupts or SystemCoreClock/Freq

	int i = 0;
	
	initializeFilter(filter); // initialize the moving average filter

		// A PWN of half a period
		if (pwm_on == 1)
		{
			// Increase the Systick frequency for pwm (we no longer see changes after 100 Hertz)
			SysTick_Config(SystemCoreClock/PWM);
			
			while(1)
			{
				// Wait for interrupt
				while(!ticks);
				
				// Set ticks to 0
				ticks = 0;
				
				// Increment the duty cycle count
				pwm_counter++;
				
				// Set the bit
				if (current_cycle_length > pwm_counter)
				{
					GPIO_WriteBit(GPIOD, GPIO_Pin_15, 0);
				}
				// Else make sure the bits are cleared
				else if (current_cycle_length <= pwm_counter)
				{
					GPIO_WriteBit(GPIOD, GPIO_Pin_15, 1);
				}
				
				if (pwm_counter == PWM)
				{
					// Reset duty cycle counter to 0
					pwm_counter = 0;
					
					// Increment the amount of cycles the LED is on for
					current_cycle_length++;
					
					// Reset the LED back to low
					if (current_cycle_length == PWM)
					{
						current_cycle_length = 0;
					}
				}
				
				// Toggle bit
				GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
			}
		}
		// Normal procedure
		else
		{
			while(1){
				// Wait for an interrupt
				while(!ticks);
				
				// Decrement ticks
				ticks = 0;
				
				// Interrupt routine
				voltage =   ((float) ADC_GetConversionValue(ADC1) / 4095.0f) * 3 ;
				
				// Temperature (in °C) = {(VSENSE – V25) / Avg_Slope} + 25
				temperature = ((voltage - V25) / avgSlope/1000)  + 25;
				
				updateFilter(filter, temperature);
				updateLED(filter->averageValue, currentLED);
				
				i++;
			}
		}
	}

void SysTick_Handler() {
	ticks = 1;
}

/* Function initializes the LED */
void initializeLed() {
	
	// Initialize LED GPIOA
// 	GPIO_InitTypeDef gpio_init_sA;
// 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
// 	GPIO_StructInit(&gpio_init_sA);
// 	gpio_init_sA.GPIO_Pin = GPIO_Pin_12;
// 	gpio_init_sA.GPIO_Mode = GPIO_Mode_OUT;
// 	gpio_init_sA.GPIO_Speed = GPIO_Speed_100MHz;
// 	gpio_init_sA.GPIO_OType = GPIO_OType_PP;
// 	gpio_init_sA.GPIO_PuPd = GPIO_PuPd_NOPULL;
// 	GPIO_Init(GPIOA, &gpio_init_sA);
	
	// Initialize LED GPIOB
	GPIO_InitTypeDef gpio_init_sB;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_StructInit(&gpio_init_sB);
	gpio_init_sB.GPIO_Pin = GPIO_Pin_13;
	gpio_init_sB.GPIO_Mode = GPIO_Mode_OUT;
	gpio_init_sB.GPIO_Speed = GPIO_Speed_100MHz;
	gpio_init_sB.GPIO_OType = GPIO_OType_PP;
	gpio_init_sB.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &gpio_init_sB);
	
	// Initialize LED GPIOC
	GPIO_InitTypeDef gpio_init_sC;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_StructInit(&gpio_init_sC);
	gpio_init_sC.GPIO_Pin = GPIO_Pin_14;
	gpio_init_sC.GPIO_Mode = GPIO_Mode_OUT;
	gpio_init_sC.GPIO_Speed = GPIO_Speed_100MHz;
	gpio_init_sC.GPIO_OType = GPIO_OType_PP;
	gpio_init_sC.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &gpio_init_sC);
	
	// Initialize LED GPIOC
	GPIO_InitTypeDef gpio_init_sD;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_StructInit(&gpio_init_sC);
	gpio_init_sD.GPIO_Pin = GPIO_Pin_15;
	gpio_init_sD.GPIO_Mode = GPIO_Mode_OUT;
	gpio_init_sD.GPIO_Speed = GPIO_Speed_100MHz;
	gpio_init_sD.GPIO_OType = GPIO_OType_PP;
	gpio_init_sD.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &gpio_init_sD);
	
	// Initialize LED GPIOA
	GPIO_InitTypeDef gpio_init_sE;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_StructInit(&gpio_init_sE);
	gpio_init_sE.GPIO_Pin = GPIO_Pin_12;
	gpio_init_sE.GPIO_Mode = GPIO_Mode_OUT;
	gpio_init_sE.GPIO_Speed = GPIO_Speed_100MHz;
	gpio_init_sE.GPIO_OType = GPIO_OType_PP;
	gpio_init_sE.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &gpio_init_sE);
	
	/* Does not seem to want to work for anything but GPIOD */
	// Toggle LEDs on and then off
	GPIO_ToggleBits(GPIOA, GPIO_Pin_12);
	GPIO_ToggleBits(GPIOB, GPIO_Pin_13);
	GPIO_ToggleBits(GPIOC, GPIO_Pin_14);
	GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
	
	GPIO_ToggleBits(GPIOA, GPIO_Pin_12);
	GPIO_ToggleBits(GPIOB, GPIO_Pin_13);
	GPIO_ToggleBits(GPIOC, GPIO_Pin_14);
	GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
}

/* Function initializes ADC and temperature sensor */
void initializeAdc() {
	ADC_InitTypeDef adc_init_s;
	ADC_CommonInitTypeDef adc_common_init_s;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	adc_common_init_s.ADC_Mode = ADC_Mode_Independent;
	adc_common_init_s.ADC_Prescaler = ADC_Prescaler_Div2;
	adc_common_init_s.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	adc_common_init_s.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&adc_common_init_s);
	
	adc_init_s.ADC_Resolution = ADC_Resolution_12b;
	adc_init_s.ADC_ScanConvMode = DISABLE;
	adc_init_s.ADC_ContinuousConvMode = DISABLE;
	adc_init_s.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	adc_init_s.ADC_DataAlign = ADC_DataAlign_Right;
	adc_init_s.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &adc_init_s);
	
	ADC_Cmd(ADC1, ENABLE);

	ADC_TempSensorVrefintCmd(ENABLE);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_TempSensor, 1, ADC_SampleTime_480Cycles);
	
	ADC_ContinuousModeCmd(ADC1, ENABLE);

	ADC_SoftwareStartConv(ADC1);
}

/* Updates the filter after each new value */
void updateFilter(movingAverageFilter *filter, float temperature) {
	
	filter->sum -= filter->buffer[filter->index];			// Subtract the entry at index from the sum
	filter->buffer[filter->index] = temperature;			// Add the next entry to the buffer
	filter->sum += filter->buffer[filter->index];			// Add the next buffer entry to the sum
	filter->index++;																	// Increment the index, if necessary wrap-around
	if (filter->index == D)
	{
		filter->index = 0;
	}
	filter->averageValue = filter->sum / D;						// Computer average value, we technically could only use sum or average value and not have both
}

/* Fills the buffer with zeroes and sets the average to 0 */
void initializeFilter(movingAverageFilter *filter) {
	int i;
	for (i = 0; i < D; i++) {
		filter->buffer[i] = 0.0;
	}
	filter->sum = 0.0;
	filter->index = 0;
}


/*	--------------------------------------------		*/
// 									GPIO_PIN_13(1)									//
//		GPIO_PIN_12(0)								GPIO_PIN_14(2)	//
//									GPIO_PIN_15(3)									//
/*	---------------------------------------------		*/
void updateLED(float temperature, short *LED) {
	

	
	static float baseTemperature;
	
	// if temperature rising and has risen more than 2 degrees
	if (temperature - baseTemperature >= LED_THRESHOLD) {
					// turn off current LED and light next in sequence. Set LED[0] to the currently set LED
					if (LED[0] == 0)
					{
						GPIO_ToggleBits(GPIOA, GPIO_Pin_12);						
						GPIO_ToggleBits(GPIOB, GPIO_Pin_13);
						LED[0]++;
					}
					else if (LED[0] == 1)
					{
						GPIO_ToggleBits(GPIOB, GPIO_Pin_13);						
						GPIO_ToggleBits(GPIOC, GPIO_Pin_14);
						LED[0]++;
					}
					else if (LED[0] == 2)
					{
						GPIO_ToggleBits(GPIOC, GPIO_Pin_14);
						GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
						LED[0]++;
					}
					else if (LED[0] == 3)
					{
						GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
						GPIO_ToggleBits(GPIOA, GPIO_Pin_12);
						LED[0] = 0;
					}
					// Set the next base temperature
					baseTemperature = temperature;
	}
	// if temperature falling and has fallen more than 2 degrees
	else if (baseTemperature - temperature >= LED_THRESHOLD) {
					// turn off current LED and light next in sequence (counter-clockwise. Set LED[0] to the currently set LED
					if (LED[0] == 0)
					{
						GPIO_ToggleBits(GPIOA, GPIO_Pin_12);						
						GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
						LED[0] = 3;
					}
					else if (LED[0] == 1)
					{
						GPIO_ToggleBits(GPIOB, GPIO_Pin_13);						
						GPIO_ToggleBits(GPIOA, GPIO_Pin_12);
						LED[0]--;
					}
					else if (LED[0] == 2)
					{
						GPIO_ToggleBits(GPIOC, GPIO_Pin_14);
						GPIO_ToggleBits(GPIOB, GPIO_Pin_13);
						LED[0]--;
					}
					else if (LED[0] == 3)
					{
						GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
						GPIO_ToggleBits(GPIOC, GPIO_Pin_14);
						LED[0]--;
					}
					// Set the next base temperature
					baseTemperature = temperature;
	}
}

void pwm()
{
	
}







