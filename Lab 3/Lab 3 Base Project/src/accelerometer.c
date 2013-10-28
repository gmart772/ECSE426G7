#include "accelerometer.h"

/**
 * @brief Initializes and starts the accelerometer. No input or output.
 */
void initAccelerometer(void) {
	LIS302DL_InitTypeDef lis302dl_InitStruct;
	
	lis302dl_InitStruct.Axes_Enable = LIS302DL_XYZ_ENABLE;
	lis302dl_InitStruct.Self_Test = LIS302DL_SELFTEST_NORMAL;
	lis302dl_InitStruct.Full_Scale = LIS302DL_FULLSCALE_2_3;
	lis302dl_InitStruct.Output_DataRate = LIS302DL_DATARATE_400;
	lis302dl_InitStruct.Power_Mode = LIS302DL_LOWPOWERMODE_ACTIVE;
	
	LIS302DL_Init(&lis302dl_InitStruct);
	
}

/**
 * @brief Gets the latest reading from the accelerometer.
 * @param values: Buffer where the accelerometer readings 
 * will be placed. The x reading is placed in [0], y in [1],
 * and z in [2].
 */
void getAcceleration(int32_t *values) {
	LIS302DL_ReadACC(values);
}

 /**  
 * @brief Calculates the x, y and z tilts angles in degrees.
 * @param acc: Buffer with the accelerometer readings.
 * x in [0], y in [1], and z in [2].
 * @param tilts: Buffer where the tilts will be placed.
 * x in [0], y in [1], and z in [2].
 */
float* getTilt(int32_t *acc, float *tilts) {
		tilts[0] = 180 * asin((acc[0] / G)) / PI; // x deviation
		tilts[1] = 180 * asin((acc[1] / G)) / PI; // y deviation
		tilts[2] = 180 * acos((acc[2] / G)) / PI; // z deviation
	
		return tilts;
}

 /**  
 * @brief Calculates the pitch angle in degrees.
 * @param acc: Buffer with the accelerometer readings.
 * x in [0], y in [1], and z in [2].
 * @return Returns the pitch in degrees as a float.
 */
float getPitch(int32_t *acc) {
	return (180 / PI) * getAlpha(acc[0], acc[1], acc[2]);
}

 /**  
 * @brief Calculates the roll angle in degrees.
 * @param acc: Buffer with the accelerometer readings.
 * x in [0], y in [1], and z in [2].
 * @return Returns the roll in degrees as a float.
 */
float getRoll(int32_t *acc) {
	return (180 / PI) * getBeta(acc[0], acc[1], acc[2]);
}