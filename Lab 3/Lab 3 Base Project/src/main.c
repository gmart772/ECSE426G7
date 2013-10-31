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

int main()
{
	int32_t values[3];
//	float tilts[3];
	float pitch, roll;
	uint8_t currentState = ACCELEROMETER;

	initAccelerometer();
	
	movingAverageFilter filterX;
	movingAverageFilter filterY;
	movingAverageFilter filterZ;
	
	initializeFilter(&filterX);
	initializeFilter(&filterY);
	initializeFilter(&filterZ);
	
	initTimer();
	initTimer4();
	initializeTimerPin();
	
	initLeds();
	
	PWM_counter = 0;
	PWM_hold_high_counter = 0;
	PWM_hold_low_counter = 0;
	PWM_temp_counter = 0;
	
	configureLEDS(TIM4_PERIOD, TIM4_PERIOD, TIM4_PERIOD, TIM4_PERIOD);
	
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
				
				getAcceleration(values);
				
				updateFilter(&filterX, values[0]);
				updateFilter(&filterY, values[1]);
				updateFilter(&filterZ, values[2]);
				
				pitch = getPitch(filterX.averageValue, filterY.averageValue, filterZ.averageValue);
				roll = getRoll(filterX.averageValue, filterY.averageValue, filterZ.averageValue);
				
				flashLeds(pitch, roll);
				//printf("%d\n", (int) filterX.averageValue);
				//printf("%d\n", (int) filterY.averageValue);
				//printf("%d\n\n", (int) filterZ.averageValue);
				
				printf("Pitch: %f\n", pitch);
				printf("Roll: %f\n\n", roll);
			
			}
			else {
				while(timerInterrupt != TIMEOUT_OCCURRED);
				timerInterrupt = NO_TIMEOUT;
				
				PWM_temp_counter = ((TIM4_PERIOD)/100)*PWM_counter;
				configureLEDS(PWM_temp_counter, PWM_temp_counter, PWM_temp_counter, PWM_temp_counter);
				
				if (PWM_counter == 0 && PWM_hold_low_counter < PWM_hold_time)
				{
					PWM_hold_low_counter++;
				}
				else if (PWM_counter < 100)
				{
					PWM_counter += 1;
					PWM_hold_low_counter = 0;
				}
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


