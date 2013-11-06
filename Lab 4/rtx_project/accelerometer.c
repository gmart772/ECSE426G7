#include "accelerometer.h"

/**
 * @brief Initializes and starts the accelerometer. No input or output.
 * Iniitializes the calibration matrix.
 */
void initAccelerometer(void) {
	
	EXTI_InitTypeDef EXTI_InitTypeDefStruct;
  NVIC_InitTypeDef NVIC_InitStruct;
	GPIO_InitTypeDef GPIO_InitStructure;
	LIS302DL_InitTypeDef lis302dl_InitStruct;
	LIS302DL_InterruptConfigTypeDef LIS302DL_InterruptConfigTypeDefStruct;
		
	uint8_t clickInterrupt 	= 0x07;
	uint8_t zThreshold			= 0xFF;
	uint8_t xyThreshold			= 0xFF;
	uint8_t timeLimit 			= 0xFF;   
	
	lis302dl_InitStruct.Axes_Enable = LIS302DL_XYZ_ENABLE;
	lis302dl_InitStruct.Self_Test = LIS302DL_SELFTEST_NORMAL;
	lis302dl_InitStruct.Full_Scale = LIS302DL_FULLSCALE_2_3;
	lis302dl_InitStruct.Output_DataRate = LIS302DL_DATARATE_100;
	lis302dl_InitStruct.Power_Mode = LIS302DL_LOWPOWERMODE_ACTIVE;
	
	LIS302DL_Init(&lis302dl_InitStruct);
		
	LIS302DL_InterruptConfigTypeDefStruct.Latch_Request = LIS302DL_INTERRUPTREQUEST_LATCHED;
  LIS302DL_InterruptConfigTypeDefStruct.SingleClick_Axes = LIS302DL_CLICKINTERRUPT_XYZ_ENABLE;
  LIS302DL_InterruptConfigTypeDefStruct.DoubleClick_Axes = LIS302DL_DOUBLECLICKINTERRUPT_XYZ_DISABLE;
  LIS302DL_InterruptConfig(&LIS302DL_InterruptConfigTypeDefStruct);
	
	uint8_t latency = 0x7F;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	/* Configure PA0 pin as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
	
  //LIS302DL_Write(&enableClickZ, LIS302DL_CLICK_SRC_REG_ADDR, 2);
	
	LIS302DL_Write(&clickInterrupt,LIS302DL_CTRL_REG3_ADDR, 1);
	LIS302DL_Write(&zThreshold, LIS302DL_CLICK_THSZ_REG_ADDR, 1);
	LIS302DL_Write(&xyThreshold, LIS302DL_CLICK_THSY_X_REG_ADDR, 1);
	LIS302DL_Write(&timeLimit, LIS302DL_CLICK_TIMELIMIT_REG_ADDR, 1);
	
  LIS302DL_Write(&latency, LIS302DL_CLICK_LATENCY_REG_ADDR, 1);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource0);

	EXTI_InitTypeDefStruct.EXTI_Line = EXTI_Line0;
  EXTI_InitTypeDefStruct.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitTypeDefStruct.EXTI_Trigger = EXTI_Trigger_Rising;  
  EXTI_InitTypeDefStruct.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitTypeDefStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);

	resetLatch();
	
	correctionMatrix[0][0] = 1.0479;
	correctionMatrix[0][1] = -0.0188;
	correctionMatrix[0][2] = 0.0064;
	correctionMatrix[1][0] = 0.0055;
	correctionMatrix[1][1] = 1.0246;
	correctionMatrix[1][2] = -0.0098;
	correctionMatrix[2][0] = -0.0203;
	correctionMatrix[2][1] = -0.0063;
	correctionMatrix[2][2] = 1.0125;
	correctionMatrix[3][0] = 0.0483;
	correctionMatrix[3][1] = -0.0641;
	correctionMatrix[3][2] = -0.0125;
	
	accFilter filterX;
	accFilter filterY;
	accFilter filterZ;
	initializeAccFilter(&filterX);
	initializeAccFilter(&filterY);
	initializeAccFilter(&filterZ);

}

/**
 * @brief Gets the latest reading from the accelerometer
 * and applies correction to it.
 * @param values: Buffer where the accelerometer readings 
 * will be placed. The x reading is placed in [0], y in [1],
 * and z in [2].
 */
void getAcceleration(int32_t *values) {
	int32_t readings[4];
	LIS302DL_ReadACC(readings);
	readings[3] = 1000;
	resetLatch();
	calibrate(values, readings);
}

 /**  
 * @brief Calculates the pitch angle in degrees.
 * @param acc: Buffer with the accelerometer readings.
 * x in [0], y in [1], and z in [2].
 * @return Returns the pitch in degrees as a float.
 */
float getPitch(int32_t accX, int32_t accY, int32_t accZ) {
	return ((180 / 3.1415926) * getAlpha(accX, accY, accZ));
}

 /**  
 * @brief Calculates the roll angle in degrees.
 * @param acc: Buffer with the accelerometer readings.
 * x in [0], y in [1], and z in [2].
 * @return Returns the roll in degrees as a float.
 */
float getRoll(int32_t accX, int32_t accY, int32_t accZ) {
	
	
	return ((180 /3.1415926) * getBeta(accX, accY, accZ));
}

void calculateTilts(void) {
		int32_t values[3];
		float pitch, roll;
	short timerInterrupt;
	
		while(timerInterrupt != TIMEOUT_OCCURRED);
				timerInterrupt = NO_TIMEOUT;
				
				// Get acceleration values
				getAcceleration(values);
				
				// Update filter
				updateAccFilter(&filterX, values[0]);
				updateAccFilter(&filterY, values[1]);
				updateAccFilter(&filterZ, values[2]);
				
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

/**
 * @brief Corrects the raw accelerometer readings 
 * using the calibration matrix.
 * @param readings: Buffer with the accelerometer readings.
 * x in [0], y in [1], and z in [2].
 * @param result: Buffer with the corrected readings.
 * x in [0], y in [1], and z in [2].
 */
void calibrate(int32_t *result, int32_t *readings) {
	int i, j;
	
	for (i = 0; i < 3; i++) {
		result[i] = 0;
		for (j = 0; j < 4; j++) {
			result[i] +=  (int32_t) (correctionMatrix[i][j] * readings[j]); 
		}
	}
	
	result[0] += X_OFFSET;
	result[1] += Y_OFFSET;
	result[2] += Z_OFFSET;
}

/**
 * @brief Resets the latch bits in the config register.
 */
void resetLatch(void) {
	uint8_t scratch[2];
	LIS302DL_Read( scratch, LIS302DL_CLICK_SRC_REG_ADDR, 2);
}


/**
 * @brief Interrupt handler for the accelerometer interrupt.
 * Sets isTapDetected to TAP_DETECTED and clears the interrupt
 * flag.
 */
void EXTI0_IRQHandler(void) {
	// Get current interrupt status	
	if (EXTI_GetITStatus(LIS302DL_SPI_INT1_EXTI_LINE) != RESET) {
		isTapDetected = TAP_DETECTED;
		EXTI_ClearITPendingBit(LIS302DL_SPI_INT1_EXTI_LINE);
	}
    //EXTI_ClearFlag(LIS302DL_SPI_INT1_EXTI_LINE);
}        