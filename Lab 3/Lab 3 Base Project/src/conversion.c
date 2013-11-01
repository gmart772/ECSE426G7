#include "conversion.h"

/** 
 * @brief Returns the value of angle Alpha, based off calibrated accelerometer readings.
 * Outputs double Alpha.
 * @param ax: The calibrated acceleration reading in the x direction.
 * @param ay: The calibrated acceleration reading in the y direction.
 * @param az: The calibrated acceleration reading in the z direction.
 */
double getAlpha(float ax, float ay, float az)
{
	// Compute alpha = arctan(ax / sqrt(ay^2 + az^2))
	return atan2(ax, sqrt(pow(ay, 2) + pow(az, 2)));
}

/** 
 * @brief Returns the value of angle Beta, based off calibrated accelerometer readings.
 * Outputs double Beta.
 * @param ax: The calibrated acceleration reading in the x direction.
 * @param ay: The calibrated acceleration reading in the y direction.
 * @param az: The calibrated acceleration reading in the z direction.
 */
double getBeta(float ax, float ay, float az)
{
	// Compute alpha = arctan(ay / sqrt(ax^2 + az^2))
	return atan2(ay, sqrt(pow(ax, 2) + pow(az, 2)));
}



