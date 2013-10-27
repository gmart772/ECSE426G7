#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "accelerometer.h"
#include "filter.h"

short interruptStatus;

#define NO_TIMEOUT 0
#define TIMEOUT_OCCURRED 1

int main()
{
	int32_t values[3];

	initAccelerometer();
	
	movingAverageFilter filterX;
	movingAverageFilter filterY;
	movingAverageFilter filterZ;
	
	initializeFilter(&filterX);
	initializeFilter(&filterY);
	initializeFilter(&filterZ);
	
	initTimer();

	
	while(1) {
		
		// Wait for a timer 3 event to occur
		while(interruptStatus != TIMEOUT_OCCURRED);
		interruptStatus = NO_TIMEOUT;
		
		getAcceleration(values);
		
		updateFilter(&filterX, values[0]);
		updateFilter(&filterY, values[1]);
		updateFilter(&filterZ, values[2]);
		
		printf("Average x: %d\n", filterX.averageValue);
		printf("Average y: %d\n", filterY.averageValue);
		printf("Average z: %d\n\n", filterZ.averageValue);
	}
	
}


