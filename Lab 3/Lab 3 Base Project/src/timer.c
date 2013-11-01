#include "timer.h"

/**
 * @brief Interrupt handler for TIM3. Resets the 
 * interrupt flag. No input or output. 
 * Toggle GPIOB Pin 11 to show the timer period on GPIOB Pin 11
 */
void TIM3_IRQHandler(void) {
	// Get current interrupt status	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
			timerInterrupt = TIMEOUT_OCCURRED;
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		
			// Toggle a GPIO Pin 
			GPIO_ToggleBits(GPIOB, GPIO_Pin_11);
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
	
	/* Set period of TIM3 */
	TIM3_TimeBaseInitStruct.TIM_Period = 65535; // us?
	TIM3_TimeBaseInitStruct.TIM_Prescaler = 0;
	TIM3_TimeBaseInitStruct.TIM_ClockDivision = 0;
	TIM_TimeBaseInit(TIM3, &TIM3_TimeBaseInitStruct);
	
	/* Set prescaler value */
	PrescalerValue = 50;
	TIM_PrescalerConfig(TIM3, PrescalerValue, TIM_PSCReloadMode_Immediate);
	
	/* Enable interrupt on TIM3 */
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	/* Enable the timer */
	TIM_Cmd(TIM3, ENABLE);
}

/** 
 * @brief Initializes GPIOB on Pin 11. This pin
 * will be used to display the frequency of TIM3
 * No input or output.
 */
void initializeTimerPin() {
	
	GPIO_InitTypeDef gpio_init_s;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_StructInit(&gpio_init_s);
	gpio_init_s.GPIO_Pin = GPIO_Pin_11;
	gpio_init_s.GPIO_Mode = GPIO_Mode_OUT;
	gpio_init_s.GPIO_Speed = GPIO_Speed_100MHz;
	gpio_init_s.GPIO_OType = GPIO_OType_PP;
	gpio_init_s.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &gpio_init_s);
}


/** 
 * @brief Initializes and starts the TIM4 timer.
 * Frequency of timer is 6MHz. Used for LEDs (output compare) and PWM.
 * No input or output.
 */
void initTimer4() {
	
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  /* Enable the TIM3 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	TIM_TimeBaseInitTypeDef TIM4_TimeBaseInitStruct;
	
	TIM4_TimeBaseInitStruct.TIM_Period = 65535; // us?    frequency = (84M)/(period * prescaler)
	TIM4_TimeBaseInitStruct.TIM_Prescaler = 0;
	TIM4_TimeBaseInitStruct.TIM_ClockDivision = 0;
	TIM4_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM4_TimeBaseInitStruct);
	
	/* Prescaler configuration */
	/* Configure TIM4 for 6MHz */
	uint16_t PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 6000000) - 1;
	TIM_PrescalerConfig(TIM4, PrescalerValue, TIM_PSCReloadMode_Immediate);
	
	/* Output Compare Timing Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 40961;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
  
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	
	//TIM_OC1Init(TIM4, &TIM_OCInitStructure);
  //TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Disable);

  /* Output Compare Timing Mode configuration: Channel2 */
  //TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 27309;
  TIM_OC2Init(TIM4, &TIM_OCInitStructure);
  //TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Disable);

  /* Output Compare Timing Mode configuration: Channel3 */
  //TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 13654;
  TIM_OC3Init(TIM4, &TIM_OCInitStructure);
  //TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Disable);

  /* Output Compare Timing Mode configuration: Channel4 */
  //TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 6826;
  TIM_OC4Init(TIM4, &TIM_OCInitStructure);
  //TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Disable);
	
	/* TIM4 enable counter */
	TIM_Cmd(TIM4, ENABLE);
	
	/* TIM4 Main Output Enable */
  TIM_CtrlPWMOutputs(TIM4, ENABLE);
}

