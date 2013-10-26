#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "accelerometer.h"
#include "filter.h"


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
	
	while(1) {
		
		getAcceleration(values);
		
	}
	
}



