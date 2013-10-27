#include "stm32f4_discovery_lis302dl.h"
#include "math.h"
#include "conversion.h"

static float PI = 3.14159265;

static float G = 1000.0; // Earth's gravity in mg

void initAccelerometer(void);
void getAcceleration(int32_t *values);
float* getTilt(int32_t *acc, float *tilts);
float getPitch(int32_t *acc);
float getRoll(int32_t *acc);

