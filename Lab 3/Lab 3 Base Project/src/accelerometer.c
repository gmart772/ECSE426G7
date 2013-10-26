#include "accelerometer.h"

void initAccelerometer() {
	LIS302DL_InitTypeDef lis302dl_InitStruct;
	
	lis302dl_InitStruct.Axes_Enable = LIS302DL_XYZ_ENABLE;
	lis302dl_InitStruct.Self_Test = LIS302DL_SELFTEST_NORMAL;
	lis302dl_InitStruct.Full_Scale = LIS302DL_FULLSCALE_2_3;
	lis302dl_InitStruct.Output_DataRate = LIS302DL_DATARATE_400;
	lis302dl_InitStruct.Power_Mode = LIS302DL_LOWPOWERMODE_ACTIVE;
	
	LIS302DL_Init(&lis302dl_InitStruct);
	
}

void getAcceleration(int32_t *values) {
	LIS302DL_ReadACC(values);
}