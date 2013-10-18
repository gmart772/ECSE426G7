#include "main.h"

/* Main function. Initializes LED and ADC. Tracks temperature and implements PWM. 
Uses the button to select between the two */
int main()
{
	initializeLed();    // GPIOD pin 12..15
	initializeButton(); // GPIOA pin 0
	initializeAdc();
	
	//uint16_t voltage;	
	int current_cycle_length = 0;
	int pwm_counter = 0;
	uint16_t pwm_on = 0, currentstate = 0, buttonPressed;
	currentLED = 0;
	
	ticks = 0;
	// Configured for 50 ms period
	// Configure SysTick to be 20Hz
	SysTick_Config(SystemCoreClock / 20);
	// NOTE: argument here must be less than 0xFFFFFF; //(24 bit timer)
	
	initializeFilter(&filter); // initialize the moving average filter	
			
			while(1)
			{
				// Read for a button press
				//buttonPressed = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
				buttonPressed = isButtonPressed();
				
				if (buttonPressed && (currentstate == TRACKING_TEMPERATURE)) {
						currentstate = DOING_PWM;	
						SysTick_Config(SystemCoreClock/PWM);
				}
				else if (buttonPressed && (currentstate == DOING_PWM)) {
						revertPinState();
						SysTick_Config(SystemCoreClock / 20);
						currentstate = TRACKING_TEMPERATURE;
				}
				
				
				// If pwm has changed (depending on change perform some reset)
				
				if (currentstate == DOING_PWM)
				{
					// Wait for interrupt
					while(!ticks);
					// Set ticks to 0
					ticks = 0;
					
					if (current_cycle_length > pwm_counter)
					{
						GPIO_WriteBit(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15, 1);
					}
					else
					{
						GPIO_WriteBit(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15, 0);
					}
					
					// Increment the duty cycle count
					pwm_counter++;
					
					if (pwm_counter > 2000)
					{
						// Reset duty cycle counter to 0
						pwm_counter = 0;
						
						// Set the current cycle to the next amount
						current_cycle_length++;
					}
					
					if (current_cycle_length == 200)
					{
						current_cycle_length = 0;
					}
				}
				else
				{
					trackTemperature();	
				}
			}
	}

void SysTick_Handler() {
	ticks = 1;
}

/* Function initializes the LEDs */
void initializeLed() {
	
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
}

/* Function initializes the Button */
void initializeButton() {
	
	// Initialize LED GPIOA
	GPIO_InitTypeDef gpio_init_s;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_StructInit(&gpio_init_s);
	gpio_init_s.GPIO_Pin = GPIO_Pin_0;
	gpio_init_s.GPIO_Mode = GPIO_Mode_IN;
	gpio_init_s.GPIO_Speed = GPIO_Speed_100MHz;
	gpio_init_s.GPIO_OType = GPIO_OType_PP;
	gpio_init_s.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &gpio_init_s);
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
	filter->sum = 0.0f;
	filter->index = 0;
}


// Updates the LED according to the temperature
/*	--------------------------------------------		*/
// 									GPIO_PIN_13(1)									//
//		GPIO_PIN_12(0)								GPIO_PIN_14(2)	//
//									GPIO_PIN_15(3)									//
/*	---------------------------------------------		*/
void updateLED(float temperature) {
	
	printf("%f\n", temperature);
		
	// if temperature rising and has risen more than 2 degrees
	if (temperature - baseTemperature >= LED_THRESHOLD) {
					// turn off current LED and light next in sequence. Set LED[0] to the currently set LED
					if (currentLED == 0)
					{					
						GPIO_WriteBit(GPIOD, GPIO_Pin_12, 0);
						GPIO_WriteBit(GPIOD, GPIO_Pin_13, 1);

						currentLED++;
					}
					else if (currentLED == 1)
					{
						GPIO_WriteBit(GPIOD, GPIO_Pin_13, 0);
						GPIO_WriteBit(GPIOD, GPIO_Pin_14, 1);
						currentLED++;
					}
					else if (currentLED == 2)
					{
						GPIO_WriteBit(GPIOD, GPIO_Pin_14, 0);
						GPIO_WriteBit(GPIOD, GPIO_Pin_15, 1);
						currentLED++;
					}
					else if (currentLED == 3)
					{
						GPIO_WriteBit(GPIOD, GPIO_Pin_15, 0);
						GPIO_WriteBit(GPIOD, GPIO_Pin_12, 1);
						currentLED = 0;
					}
					// Set the next base temperature
					baseTemperature = temperature;
	}
	// if temperature falling and has fallen more than 2 degrees
	else if (baseTemperature - temperature >= LED_THRESHOLD) {
					// turn off current LED and light next in sequence (counter-clockwise. Set LED[0] to the currently set LED
					if (currentLED == 0)
					{
						GPIO_WriteBit(GPIOD, GPIO_Pin_12, 0);
						GPIO_WriteBit(GPIOD, GPIO_Pin_15, 1);
						currentLED = 3;
					}
					else if (currentLED == 1)
					{
						GPIO_WriteBit(GPIOD, GPIO_Pin_13, 0);
						GPIO_WriteBit(GPIOD, GPIO_Pin_12, 1);
						currentLED--;
					}
					else if (currentLED == 2)
					{
						GPIO_WriteBit(GPIOD, GPIO_Pin_14, 0);
						GPIO_WriteBit(GPIOD, GPIO_Pin_13, 1);
						currentLED--;
					}
					else if (currentLED == 3)
					{
						GPIO_WriteBit(GPIOD, GPIO_Pin_15, 0);
						GPIO_WriteBit(GPIOD, GPIO_Pin_14, 1);
						currentLED--;
					}
					// Set the next base temperature
					baseTemperature = temperature;
	}
}

/* Takes a temperature reading and then applies signal processing */
void trackTemperature() {
	float voltage;
	float temperature;
	float avgSlope = 2.5 / 1000; // value from datasheet
	
	// Wait for an interrupt
	while(!ticks);
	
	// Decrement ticks
	ticks = 0;
	
	// Interrupt routine
	voltage = ((float) ADC_GetConversionValue(ADC1) / 4095.0f) * 3 ;
	
	// Temperature (in °C) = {(VSENSE – V25) / Avg_Slope} + 25
	temperature = ((voltage - V25) / avgSlope)  + 25;
	
	updateFilter(&filter, temperature);
	
	// Only update LED after X readings?
	updateLED(filter.averageValue);
}

/* Return the pins to the last state they had when temperature tracking was on */
void revertPinState() {
		switch(currentLED)
		{
		case 0: 
			GPIO_WriteBit(GPIOD, GPIO_Pin_12, 1);
			GPIO_WriteBit(GPIOD, GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15, 0);
			break;
		case 1:
			GPIO_WriteBit(GPIOD, GPIO_Pin_13, 1);
			GPIO_WriteBit(GPIOD, GPIO_Pin_12 | GPIO_Pin_14 | GPIO_Pin_15, 0);
			break;
		case 2:
			GPIO_WriteBit(GPIOD, GPIO_Pin_14, 1);
			GPIO_WriteBit(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15, 0);
			break;
		case 3:
			GPIO_WriteBit(GPIOD, GPIO_Pin_15, 1);
			GPIO_WriteBit(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14, 0);
			break;
		default:
			break;
		}
}

/* Reads the input from the button and debounces it */
int isButtonPressed() {
	uint16_t isBitSet;
	
	
	SysTick_Config(SystemCoreClock/5);
	
	isBitSet = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
	if (isBitSet) {
			while (!ticks);
			ticks = 0;
			isBitSet = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
			if (!isBitSet) {
				return 1;
			}
			else { 
				while (!ticks);
				ticks = 0;
				isBitSet = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
				if (!isBitSet) {
					return 1;
				}
				else {
					return 0;
				}
			}
		}
		else {
			return 0;
		}
	
}







