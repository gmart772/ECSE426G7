#include "timer.h"

/**
 * @brief Interrupt handler for TIM3. Resets the 
 * interrupt flag. No input or output. 
 */
void TIM3_IRQHandler(void) {
	// Get current interrupt status	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
			timerInterrupt = TIMEOUT_OCCURRED;
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		
		}
}

/** 
 * @brief Initializes and starts the TIM3 timer 
 * to generate an interrupt ever 40 ms (25 Hz).
 * No input or output.
 */
void initTimer(short mode) {
	NVIC_InitTypeDef NVIC_InitStructure;
	uint16_t PrescalerValue = 0;

  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /* Enable the TIM3 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	
	TIM_TimeBaseInitTypeDef TIM3_TimeBaseInitStruct;
	
	if (mode == ACCELEROMETER)
	{
		TIM3_TimeBaseInitStruct.TIM_Period = 65535; // us?
		TIM3_TimeBaseInitStruct.TIM_Prescaler = 0;
		TIM3_TimeBaseInitStruct.TIM_ClockDivision = 0;
		
		TIM_TimeBaseInit(TIM3, &TIM3_TimeBaseInitStruct);
		
		PrescalerValue = 52;
	}
	else if (mode == PWM)
	{
		TIM3_TimeBaseInitStruct.TIM_Period = 65535; // us?
		TIM3_TimeBaseInitStruct.TIM_Prescaler = 0;
		TIM3_TimeBaseInitStruct.TIM_ClockDivision = 0;
		
		TIM_TimeBaseInit(TIM3, &TIM3_TimeBaseInitStruct);
		
		PrescalerValue = 26;
	}
	
	TIM_PrescalerConfig(TIM3, PrescalerValue, TIM_PSCReloadMode_Immediate);
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	TIM_Cmd(TIM3, ENABLE);
}

