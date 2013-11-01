#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "stm32f4_discovery_lis302dl.h"
#include "math.h"

double getAlpha(float ax, float ay, float az);
double getBeta(float ax, float ay, float az);