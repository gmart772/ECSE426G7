#include "conversion.h"

#define XOFFSET 30
#define YOFFSET 15
#define ZOFFSET 20

#define SAFETY_NET 30

#define FREEFALL_THRESHOLD (XOFFSET + YOFFSET + ZOFFSET + SAFETY_NET)

int isFreeFallDimensionless(float af);
int isFreeFall(float ax, float ay, float az);

