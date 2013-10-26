#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "accelerometer.h"
#include "filter.h"

ITStatus interruptStatus;

int main()
{
	int32_t values[3];

	initAccelerometer();
	TIM_TimeBaseInitTypeDef TIM2_TimeBaseInitStruct, TIM3_TimeBaseInitStruct;
	TIM_TimeBaseStructInit(&TIM2_TimeBaseInitStruct);
	TIM_TimeBaseStructInit(&TIM3_TimeBaseInitStruct);
	
	//TIM2_TimeBaseInitStruct.TIM_Period = 10; // ms?
	//TIM2_TimeBaseInitStruct.TIM_Prescaler = 0;
	TIM3_TimeBaseInitStruct.TIM_Period = 5; // ms?
	TIM3_TimeBaseInitStruct.TIM_Prescaler = 65000;
	TIM_TimeBaseInit(TIM3, &TIM3_TimeBaseInitStruct);
	
	//
	TIM_ITConfig(TIM3, TIM_IT_Trigger, ENABLE);
	
	//TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	
	movingAverageFilter filterX;
	movingAverageFilter filterY;
	movingAverageFilter filterZ;
	
	initializeFilter(&filterX);
	initializeFilter(&filterY);
	initializeFilter(&filterZ);
	
	while(1) {
		
		// Wait for a timer 3 event to occur
		while(TIM3_IRQ() != 1);
		
		getAcceleration(values);
		
		updateFilter(&filterX, values[0]);
		updateFilter(&filterY, values[1]);
		updateFilter(&filterZ, values[2]);
		
		printf("Average x: %d\n", filterX.averageValue);
		printf("Average y: %d\n", filterY.averageValue);
		printf("Average z: %d\n\n", filterZ.averageValue);
	}
	
}

int TIM3_IRQ()
{
	// Get current interrupt status
	interruptStatus = TIM_GetITStatus(TIM3, TIM_IT_Trigger);
	
	if (interruptStatus == RESET)
	{
		return 0;
	}
	// Interrupt is set
	else
	{
		// Clear the TIM flag
		TIM_ClearFlag(TIM3, TIM_FLAG_Trigger);
		return 1;
	}
}



