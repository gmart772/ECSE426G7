#include "math.h"
#include "accelerometer.h"

#define PWM_hold_time 13

static int PWM_counter = 0;
static int PWM_hold_high_counter = 0;
static int PWM_hold_low_counter = 0;
static int PWM_temp_counter = 0;
	
static int PWM_quadratic = 0.0;
