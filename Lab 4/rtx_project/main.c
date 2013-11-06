#include "arm_math.h"

#include "stm32f4xx.h"
#include "cmsis_os.h"
#include "swPwm.h"
#include "hwPwm.h"
//#include "threads.h"
//#include "temperatureTracking.h"
//#include "accelerometer.h"

short isTapDetected;
short timerInterrupt;
int mode;

//osMailQDef(mail, 1, LEDs); 


/*!
 @brief Thread to perform menial tasks such as switching LEDs
 @param argument Unused
 */
void accelerometer(void const * argument);
void hardwarePwm(void const * argument);
void temperatureTracking(void const * argument);
void softwarePwm(void const * argument);
void modeControl(void const * argument);


//! Thread structure for above thread
osThreadDef(accelerometer, osPriorityNormal, 1, 0);
osThreadDef(hardwarePwm, osPriorityNormal, 1, 0);
osThreadDef(temperatureTracking, osPriorityNormal, 1, 0);
osThreadDef(softwarePwm, osPriorityNormal, 1, 0);
osThreadDef(modeControl, osPriorityNormal, 1, 0);

// Create a thread definition for the LEDs
//osThreadDef(leds, osPriorityNormal, 1, 0);

/*!
 @brief Program entry point
 */
int main (void) {


	// Start thread
//	mail = osMailCreate(osMailQ(mail), NULL);

	mode = ACCELEROMETER_MODE;
	initializeButton();
	initTimer4();
	initLeds();
	initTimer();	


	tid_thread5 = osThreadCreate(osThread(modeControl), NULL);
	tid_thread1 = osThreadCreate(osThread(accelerometer), NULL);
	tid_thread2 = osThreadCreate(osThread(hardwarePwm), NULL);
	

//	tid_thread3 = osThreadCreate(osThread(temperatureTracking), NULL);
//	tid_thread4 = osThreadCreate(osThread(softwarePwm), NULL);
	
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

void accelerometer(void const *argument) {
	initAccelerometer();
	calculateTilts();
}

void hardwarePwm(void const *argument) {
		hwPwm();
}

void softwarePwm(void const *argument) {
		swPwm();
}

void temperatureTracking(void const *argument) {
		trackTemperature();
}

void modeControl(void const *argument) {
		initializeButton();
	while (1) {
		if (isButtonPressed()) {
			if (mode == ACCELEROMETER_MODE) {
				mode = HW_PWM_MODE;
			}
			else if (mode == HW_PWM_MODE) {
				mode = ACCELEROMETER_MODE;
			}
			else if (mode == TEMPERATURE_MODE) {
				mode = SW_PWM_MODE;
			}
			else if (mode == SW_PWM_MODE) {
				mode = TEMPERATURE_MODE;
			}
		}
	}
}

