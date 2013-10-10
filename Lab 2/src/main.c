#include <stdio.h>
#include "stm32f4xx.h"

#include "stm32f4xx_adc.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

//#include "gpio_example.h"
float V25 = 0.76;

static volatile uint_fast16_t ticks;

int main()
{
	GPIO_InitTypeDef gpio_init_s;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_StructInit(&gpio_init_s);
	gpio_init_s.GPIO_Pin = GPIO_Pin_12;
	gpio_init_s.GPIO_Mode = GPIO_Mode_OUT;
	gpio_init_s.GPIO_Speed = GPIO_Speed_100MHz;
	gpio_init_s.GPIO_OType = GPIO_OType_PP;
	gpio_init_s.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &gpio_init_s);
	
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

	//uint16_t voltage;
	float voltage;
	float temperature;
	float avgSlope = 2.5;
	
	ticks = 0;
	// Configured for 50 ms period
	// Configure SysTick to be 20Hz
	// NOTE: argument here must be less than 0xFFFFFF; //(24 bit timer)
	SysTick_Config(SystemCoreClock / 20); // Number of ticks between two interrupts or SystemCoreClock/Freq
	
	int i = 0;
	
	while(1){
		
		// Wait for an interrupt
		while(!ticks);
		
		// Decrement ticks
		ticks = 0;
		
		// Interrupt routine
		// Temperature (in °C) = {(VSENSE – V25) / Avg_Slope} + 25
		
		/* ALL VALUES SCALED UP TO COMPENSATE FOR INT */
		voltage =   ((float) ADC_GetConversionValue(ADC1) / 4095.0f) * 3 ;
		
		temperature = ((voltage - V25) / avgSlope)  + 25;
		
		//printf("Temp is : %d", voltage);
		
		//GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
		//printf("This is a TEST?");
		i++;
	}
	
}

void SysTick_Handler() {
	ticks = 1;
}





