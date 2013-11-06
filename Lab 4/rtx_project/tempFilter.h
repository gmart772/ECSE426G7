#define D 10

typedef struct temperatureFilter { // Struct to keep track of filter internals
	float buffer[D]; // buffer
	float sum; // sum of values in the buffer
	float averageValue; // average of the values in the buffer
	short index; 				// Index of next buffer entry
	
} temperatureFilter;

void initializeTempFilter(temperatureFilter *filter);
void updateTempFilter(temperatureFilter *filter, float acc);