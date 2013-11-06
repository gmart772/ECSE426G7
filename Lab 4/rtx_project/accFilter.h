#define D 10

typedef struct accFilter { // Struct to keep track of filter internals
	int buffer[D]; // buffer
	int sum; // sum of values in the buffer
	int averageValue; // average of the values in the buffer
	short index; 				// Index of next buffer entry
	
} accFilter;

void initializeAccFilter(accFilter *filter);
void updateAccFilter(accFilter *filter, int acc);