#include "arm_math.h"
#include "stm32f4xx.h"
#include "cmsis_os.h"
#include "swPwm.h"
#include "hwPwm.h"
#include "led.h"

short isTapDetected;
short timerInterrupt;
int mode;

osThreadId tid_thread1, tid_thread2, tid_thread3, tid_thread4, tid_thread5, tid_thread6;

osMutexId modeMutex;
osMutexDef(modeMutex);

//! Thread structure for above thread
osThreadDef(accelerometer, osPriorityNormal, 1, 0);
osThreadDef(hardwarePwm, osPriorityNormal, 1, 0);
osThreadDef(temperatureTracking, osPriorityNormal, 1, 0);
osThreadDef(softwarePwm, osPriorityNormal, 1, 0);
osThreadDef(buttonControl, osPriorityNormal, 1, 0);
osThreadDef(tapDetection, osPriorityNormal, 1, 0);


/*!
 @brief Program entry point
 */
int main (void) {	
	// Set the default mode
	mode = ACCELEROMETER_MODE;
	
	// Initalize all periphs from previous labs
	initializeButton();
	initializeAdc();
	initTimer2();
	initTimer4();
	initLeds();
	initTimer();	

	// Create mutux for the mode
	modeMutex = osMutexCreate(osMutex(modeMutex));
	
	// Start thread all threads
	tid_thread5 = osThreadCreate(osThread(buttonControl), NULL);
	tid_thread6 = osThreadCreate(osThread(tapDetection), NULL);
	tid_thread1 = osThreadCreate(osThread(accelerometer), NULL);
	tid_thread4 = osThreadCreate(osThread(softwarePwm), NULL);
	tid_thread3 = osThreadCreate(osThread(temperatureTracking), NULL);
	tid_thread2 = osThreadCreate(osThread(hardwarePwm), NULL);

	// The below doesn't really need to be in a loop
	while(1){
		osDelay(osWaitForever);
	}
}

