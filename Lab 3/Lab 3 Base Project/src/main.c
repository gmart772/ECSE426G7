#include <stdio.h>
//#include "stm32f4xx.h"
//#include "stm32f4xx_conf.h"

#include "accelerometer.h"
#include "filter.h"
#include "timer.h"
#include "led.h"

#define ACCELEROMETER 0
#define PWM 1

#define PWM_hold_time 13

short timerInterrupt;
short timerInterrupt4;
short isTapDetected;

int PWM_counter;
int PWM_hold_high_counter;
int PWM_hold_low_counter;
int PWM_temp_counter;

float PWM_quadratic;

int main()
{
	int32_t values[3];
	float pitch, roll;
	
	uint8_t currentState = ACCELEROMETER;

	// Init accelerometer
	initAccelerometer();
	
	// Create 3 moving filters, 1 per axis and init them
	movingAverageFilter filterX;
	movingAverageFilter filterY;
	movingAverageFilter filterZ;
	initializeFilter(&filterX);
	initializeFilter(&filterY);
	initializeFilter(&filterZ);
	
	// Init and start TIM3 (used for acceleration, 25Hz clock output on GPIOB Pin 11) and TIM4 (used for PWM and LED control)
	initTimer();
	initTimer4();
	
	// Init all LEDs
	initLeds();
	initializeTimerPin();
	
	// Initialize PWM values to 0
	PWM_counter = 0;
	PWM_hold_high_counter = 0;
	PWM_hold_low_counter = 0;
	PWM_temp_counter = 0;
	
	PWM_quadratic = 0.0;
	
	configureLEDS(0, 0, 0, 0);
	
	while(1) {
		
		if ((isTapDetected == TAP_DETECTED) && (currentState == ACCELEROMETER)) {
			isTapDetected = NO_TAP_DETECTED;
			currentState = PWM;
			resetLatch();
			configureLEDS(0, 0, 0, 0);
		}
		else if ((isTapDetected == TAP_DETECTED) && (currentState == PWM)) {
			isTapDetected = NO_TAP_DETECTED;
			currentState = ACCELEROMETER;
			resetLatch();		
			configureLEDS(0, 0, 0, 0);
		}
		
		if (currentState == ACCELEROMETER) {
				// Wait for a timer 3 event to occur
				while(timerInterrupt != TIMEOUT_OCCURRED);
				timerInterrupt = NO_TIMEOUT;
				
				// Get acceleration values
				getAcceleration(values);
				
				// Update filter
				updateFilter(&filterX, values[0]);
				updateFilter(&filterY, values[1]);
				updateFilter(&filterZ, values[2]);
				
				// Calculate pitch and roll from the filter values
				pitch = getPitch(filterX.averageValue, filterY.averageValue, filterZ.averageValue);
				roll = getRoll(filterX.averageValue, filterY.averageValue, filterZ.averageValue);
				
				// Update LEDs
				flashLeds(pitch, roll);
				//printf("%d\n", (int) filterX.averageValue);
				//printf("%d\n", (int) filterY.averageValue);
				//printf("%d\n\n", (int) filterZ.averageValue);
				
				// Display pitch and roll on debugger
				printf("Pitch: %f\n", pitch);
				printf("Roll: %f\n\n", roll);
			
			}
			else {
				// Wait for a timer 3 event to occur
				while(timerInterrupt != TIMEOUT_OCCURRED);
				timerInterrupt = NO_TIMEOUT;
				
				// PWM_counter ranges from 0-100
				// PWM_quadratic is used to ramp the value of PWM_temp_counter based on this value of PWM_counter
				PWM_quadratic = pow(PWM_counter, 2)/100;
				PWM_temp_counter = ((TIM4_PERIOD)/100)*PWM_quadratic;
				
				// Update the LEDs for the new duty cycle
				configureLEDS(PWM_temp_counter, PWM_temp_counter, PWM_temp_counter, PWM_temp_counter);
				
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
	
}


