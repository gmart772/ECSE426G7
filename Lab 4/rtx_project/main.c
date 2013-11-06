#include "arm_math.h"

#include "stm32f4xx.h"
#include "cmsis_os.h"
#include "swPwm.h"
#include "hwPwm.h"
//#include "temperatureTracking.h"
//#include "accelerometer.h"

short isTapDetected;
short timerInterrupt;

/*!
 @brief Thread to perform menial tasks such as switching LEDs
 @param argument Unused
 */
void thread(void const * argument);

//! Thread structure for above thread
osThreadDef(thread, osPriorityNormal, 1, 0);

/*!
 @brief Program entry point
 */
int main (void) {
	// ID for thread
	osThreadId tid_thread1;
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	// Start thread
	tid_thread1 = osThreadCreate(osThread(thread), NULL);

	// The below doesn't really need to be in a loop
	while(1){
		osDelay(osWaitForever);
	}
}

void thread (void const *argument) {
	while(1){
		osDelay(1000);
		GPIOD->BSRRL = GPIO_Pin_12;
		osDelay(1000);
		GPIOD->BSRRH = GPIO_Pin_12;
	}
}
