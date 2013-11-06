#include "swPwm.h"

void swPwm(void) {
			// Wait for interrupt
		while(!ticks);
		// Set ticks to 0
		ticks = 0;
		
		if (current_cycle_length >= pwm_counter)
		{
			GPIO_WriteBit(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15, 1);
		}
		else
		{
			GPIO_WriteBit(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15, 0);
		}
		
		// Increment the duty cycle count
		pwm_counter++;
		
		if (pwm_counter > 1500)
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

// void SysTick_Handler() {
// 	ticks = 1;
// }
