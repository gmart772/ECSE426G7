#include <stdio.h>
//#include "stm32f4xx.h"
//#include "stm32f4xx_conf.h"

#include "accelerometer.h"
#include "filter.h"
#include "timer.h"
#include "led.h"

#define ACCELEROMETER 0
#define PWM 1

short timerInterrupt;
short isTapDetected;

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
	initLeds();

	
	while(1) {
		
		if ((isTapDetected == TAP_DETECTED) && (currentState == ACCELEROMETER)) {
			isTapDetected = NO_TAP_DETECTED;
			currentState = PWM;
			uint8_t scratch[2];
			LIS302DL_Read( scratch, LIS302DL_CLICK_SRC_REG_ADDR, 2);

		}
		else if ((isTapDetected == TAP_DETECTED) && (currentState == PWM)) {
			isTapDetected = NO_TAP_DETECTED;
			currentState = ACCELEROMETER;
			uint8_t scratch[2];
			LIS302DL_Read( scratch, LIS302DL_CLICK_SRC_REG_ADDR, 2);
			
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
		//		printf("%d\n", (int) filterX.averageValue);
		//		printf("%d\n", (int) filterY.averageValue);
		//		printf("%d\n\n", (int) filterZ.averageValue);
				
		//		printf("Pitch: %f\n", pitch);
		//		printf("Roll: %f\n\n", roll);
			
			}
			else {
				//Turn all LEDs off until we get PWM started
				// Debug code to detect tap
				
				GPIO_WriteBit(GPIOD, GPIO_Pin_13, 0);
				GPIO_WriteBit(GPIOD, GPIO_Pin_15, 0);
				GPIO_WriteBit(GPIOD, GPIO_Pin_12, 0);
				GPIO_WriteBit(GPIOD, GPIO_Pin_14, 0);
				
				
			}
		
	}
	
}


