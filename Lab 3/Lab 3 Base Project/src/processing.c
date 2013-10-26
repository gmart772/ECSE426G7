// Returns the freefall acceleration value
float getFreeFallAcceleration(float ax, float ay, float az)
{
	// Compute the freefall acceleration
	return sqrt(pow(ax, 2) + pow(ay, 2) + pow(az, 2));
}

// Returns 1 if freefall mode is detected, 0 otherwise
int isFreeFall(float af)
{
	// If we are below the threshold, we are in freefall mode
	if (af < FREEFALL_THRESHOLD)
	{
		return 1;
	}
	
	// Not in freefall
	return 0;
}

// Returns 1 if freefall mode is detected, 0 otherwise
int isFreeFall(float ax, float ay, float az)
{
	float af = sqrt(pow(ax, 2) + pow(ay, 2) + pow(az, 2));
	
	// If we are below the threshold, we are in freefall mode
	if (af < FREEFALL_THRESHOLD)
	{
		return 1;
	}
	
	// Not in freefall
	return 0;
}

