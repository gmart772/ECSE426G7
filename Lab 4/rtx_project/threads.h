#include "cmsis_os.h"

extern osThreadId tid_thread1, tid_thread2, tid_thread3, tid_thread4, tid_thread5, tid_thread6;

#define ACCELEROMETER_MODE  0
#define HW_PWM_MODE  				1
#define TEMPERATURE_MODE	  2
#define SW_PWM_MODE					3

extern int mode;
extern osMutexId modeMutex;

/*!
 @brief Thread to perform menial tasks such as switching LEDs
 @param argument Unused
 */
void accelerometer(void const * argument);
void hardwarePwm(void const * argument);
void temperatureTracking(void const * argument);
void softwarePwm(void const * argument);
void buttonControl(void const * argument);
void tapDetection(void const * argument);
void Timer1_Callback(void const *argument);
