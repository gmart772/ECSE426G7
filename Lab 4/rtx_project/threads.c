#include "threads.h"

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

void buttonControl(void const *argument) {

	while (1) {
		if (isButtonPressed()) {
		osMutexWait(modeMutex, osWaitForever);
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
				revertPinState();
			}
			osMutexRelease(modeMutex);
		//	osDelay(1);
		}
	}
}

void tapDetection(void const *argument) {
	while (1) {
		osSignalWait(1, osWaitForever);
	
		osMutexWait(modeMutex, osWaitForever);
		if ((mode == ACCELEROMETER_MODE) || (mode == HW_PWM_MODE)) {
			mode = TEMPERATURE_MODE;
			revertPinState();
		}
		else if ((mode == TEMPERATURE_MODE) || (mode == SW_PWM_MODE)) {
			mode = ACCELEROMETER_MODE;
		}
		osMutexRelease(modeMutex);
		//osDelay(1);
	}
}