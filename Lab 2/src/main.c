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
	
	ticks = 0;
	// Configured for 50 ms period
	// Configure SysTick to be 20Hz
	// NOTE: argument here must be less than 0xFFFFFF; //(24 bit timer)
	SysTick_Config(SystemCoreClock / 20); // Number of ticks between two interrupts or SystemCoreClock/Freq
	
	int i = 0;
	
	initializeFilter(filter); // initialize the moving average filter
	while(1){
		
		// Wait for an interrupt
		while(!ticks);
		
		// Decrement ticks
		ticks = 0;
		
		// Interrupt routine
		voltage =   ((float) ADC_GetConversionValue(ADC1) / 4095.0f) * 3 ;
		
		// Temperature (in °C) = {(VSENSE – V25) / Avg_Slope} + 25
		temperature = ((voltage - V25) / avgSlope)  + 25;
		
		updateFilter(filter, temperature);
		updateLED(filter->averageValue);
		
		i++;
	}
	
}

void SysTick_Handler() {
	ticks = 1;
}

/* Function initializes the LED */
void initializeLed() {
	GPIO_InitTypeDef gpio_init_s;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_StructInit(&gpio_init_s);
	gpio_init_s.GPIO_Pin = GPIO_Pin_12;
	gpio_init_s.GPIO_Mode = GPIO_Mode_OUT;
	gpio_init_s.GPIO_Speed = GPIO_Speed_100MHz;
	gpio_init_s.GPIO_OType = GPIO_OType_PP;
	gpio_init_s.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &gpio_init_s);
	
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
	int i;
	float sum = temperature; // sum of the values to compute the average
	for (i = 0; i < D - 1; i++) {
		filter->buffer[i] = filter->buffer[i + 1]; // shift the buffer values to clear the final entry
		sum += filter->buffer[i + 1]; // update the sum to prepare to get the average
	}
	filter->buffer[D - 1] = temperature; // add the current temperature to the end of the buffer
	
	filter->averageValue = sum / D; // compute the average value and update the value in the struct
}

/* Fills the buffer with zeroes and sets the average to 0 */
void initializeFilter(movingAverageFilter *filter) {
	int i;
	for (i = 0; i < D; i++) {
		filter->buffer[i] = 0.0;
	}
	filter->averageValue = 0.0;
	
}

void updateLED(float temperature) {
	static float baseTemperature;
	
	// if temperature rising and has risen more than 2 degrees
	if (temperature - baseTemperature >= LED_THRESHOLD) {
					// turn off current LED
					// light next in clockwise sequence
					baseTemperature = temperature;
	}
	// if temperature falling and has fallen more than 2 degrees
	else if (baseTemperature - temperature >= LED_THRESHOLD) {
					// turn off current LED
					// light next in counterclockwise sequence
					baseTemperature = temperature;
	}
}






