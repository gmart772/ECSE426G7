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
 * @brief Interrupt handler for TIM4. Resets the 
 * interrupt flag. No input or output. 
 */
void TIM4_IRQHandler(void) {
	// Get current interrupt status	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) {
			timerInterrupt = TIMEOUT_OCCURRED;
			TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		}
}

/** 
 * @brief Initializes and starts the TIM3 timer 
 * to generate an interrupt ever 40 ms (25 Hz).
 * No input or output.
 */
void initTimer() {
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
	
	TIM3_TimeBaseInitStruct.TIM_Period = 65535; // us?
	TIM3_TimeBaseInitStruct.TIM_Prescaler = 0;
	TIM3_TimeBaseInitStruct.TIM_ClockDivision = 0;
	
	TIM_TimeBaseInit(TIM3, &TIM3_TimeBaseInitStruct);
	
	PrescalerValue = 50;
	
	TIM_PrescalerConfig(TIM3, PrescalerValue, TIM_PSCReloadMode_Immediate);
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	TIM_Cmd(TIM3, ENABLE);
}

/** 
 * @brief Initializes and starts the TIM3 timer 
 * to generate an interrupt ever 40 ms (25 Hz).
 * No input or output.
 */
void initTimer4() {
	NVIC_InitTypeDef NVIC_InitStructure;
	uint16_t PrescalerValue = 0;

  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  /* Enable the TIM3 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	
	TIM_TimeBaseInitTypeDef TIM4_TimeBaseInitStruct;
	
	TIM4_TimeBaseInitStruct.TIM_Period = 65535; // us?
	TIM4_TimeBaseInitStruct.TIM_Prescaler = 0;
	TIM4_TimeBaseInitStruct.TIM_ClockDivision = 0;
	
	TIM_TimeBaseInit(TIM4, &TIM4_TimeBaseInitStruct);
	
	PrescalerValue = 2;
	
	TIM_PrescalerConfig(TIM4, PrescalerValue, TIM_PSCReloadMode_Immediate);
	
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	
	TIM_Cmd(TIM4, ENABLE);
}

