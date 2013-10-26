#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "stm32f4_discovery_lis302dl.h"




int main()
{
	LIS302DL_InitTypeDef lis302dl_InitStruct;
	
	lis302dl_InitStruct.Axes_Enable = LIS302DL_XYZ_ENABLE;
	lis302dl_InitStruct.Self_Test = LIS302DL_SELFTEST_NORMAL;
	
	LIS302DL_Init(&lis302dl_InitStruct);
	
	while(1) {

		
		
	}
	
}


