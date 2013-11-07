#include "cmsis_os.h"

extern osThreadId tid_thread1, tid_thread2, tid_thread3, tid_thread4, tid_thread5, tid_thread6;

//static	osMailQId mail

#define ACCELEROMETER_MODE  0
#define HW_PWM_MODE  				1
#define TEMPERATURE_MODE	  2
#define SW_PWM_MODE					3

extern int mode;


