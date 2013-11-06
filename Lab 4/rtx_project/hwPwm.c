#include "hwPwm.h"

void hwPwm() {
					// Wait for a timer 3 event to occur
	while (1) {
				osSignalWait(1, 1);
				
				// PWM_counter ranges from 0-100
				// PWM_quadratic is used to ramp the value of PWM_temp_counter based on this value of PWM_counter
				PWM_quadratic = pow(PWM_counter, 2)/100;
				PWM_temp_counter = ((TIM4_PERIOD)/100)*PWM_quadratic;
				
				// Update the LEDs for the new duty cycle
				if (mode == HW_PWM_MODE) {
					configureLEDS(PWM_temp_counter, PWM_temp_counter, PWM_temp_counter, PWM_temp_counter);
				}
				
				// We hold the lights off for PWM_hold_time cycles when PWM_counter = 0 (13 cycles ~ 1/2 second)
				if (PWM_counter == 0 && PWM_hold_low_counter < PWM_hold_time)
				{
					PWM_hold_low_counter++;
				}
				// Increment PWM_counter
				else if (PWM_counter < 100)
				{
					PWM_counter += 1;
					PWM_hold_low_counter = 0;
				}
				// We hold the lights on for PWM_hold_time cycles when PWM_counter = 100 (13 cycles ~ 1/2 second)
				else if (PWM_hold_high_counter == PWM_hold_time)
				{
					PWM_counter = 0;
					PWM_hold_high_counter = 0;
				}
				else
				{
					PWM_hold_high_counter++;
				}
			}

}