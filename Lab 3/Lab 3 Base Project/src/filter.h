#define D 1

typedef struct movingAverageFilter { // Struct to keep track of filter internals
	int buffer[D]; // buffer
	int sum; // sum of values in the buffer
	int averageValue; // average of the values in the buffer
	short index; 				// Index of next buffer entry
	
} movingAverageFilter;

void initializeFilter(movingAverageFilter *filter);
void updateFilter(movingAverageFilter *filter, int acc);