#include "swPwm.h"

void swPwm(void) {
 int pwm_counter = 0;

	
	while (1) {
			// Wait for interrupt
		osSignalWait(1, osWaitForever);

		
		osMutexWait(modeMutex, osWaitForever);
		if (mode == SW_PWM_MODE) {
				configureLEDS(pwm_counter * (TIM4_PERIOD / 80), pwm_counter * (TIM4_PERIOD / 80),
			pwm_counter * (TIM4_PERIOD / 80), pwm_counter * (TIM4_PERIOD / 80));
		}
		osMutexRelease(modeMutex);
		
		// Increment the duty cycle count
		pwm_counter++;
		
		if (pwm_counter > 80)
		{
			// Reset duty cycle counter to 0
			pwm_counter = 0;
		}
	}
}

// void SysTick_Handler() {
// 	ticks = 1;
// }
