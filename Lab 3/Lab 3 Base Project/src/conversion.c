#include "conversion.h"

double getAlpha(float ax, float ay, float az)
{
	// Compute alpha = arctan(ax / sqrt(ay^2 + az^2))
	return atan2(ax, sqrt(pow(ay, 2) + pow(az, 2)));
}

double getBeta(float ax, float ay, float az)
{
	// Compute alpha = arctan(ay / sqrt(ax^2 + az^2))
	return atan2(ay, sqrt(pow(ax, 2) + pow(az, 2)));
}

void calibrateReading(float *ax, float *ay)
{
	float ACCX = 0;
	float ACCY = 0;
	
	float OUTX = *ax;
	float OUTY = *ay;
	
	int i = 0;
	
	// Calibrate readings
	while(i < 10)
	{
		ACCX = ((OUTX - OFFX)/SENSX) - ACCY*CXY;
		ACCY = ((OUTY - OFFY)/SENSY) - ACCX*CYX;
		i++;
	}
	
	// Set the pointers to the correct return value
	*ax = ACCX;
	*ay = ACCY;
}

void getAngles(double *alpha, double *beta, float ax, float ay, float az)
{
	float *ACCX = &ax;
	float *ACCY = &ay;
	
	// Calibrate ax and ay values
	calibrateReading(ACCX, ACCY);
	
	// Calculate the angles
	*alpha = getAlpha(*ACCX, *ACCY, az);
	*beta = getBeta(*ACCX, *ACCY, az);
	
}



