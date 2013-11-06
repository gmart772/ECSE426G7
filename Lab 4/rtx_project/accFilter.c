#include "accFilter.h"

/**
 * @brief Updates the filter after each new value.
 * Updates the sum, buffer, index, and average value
 * @param filter: The movingAverageFilter to be updated.
 * @param acc: The new acceleration to input into the filter.
 */
void updateAccFilter(accFilter *filter, int acc) {
	
	filter->sum -= filter->buffer[filter->index];			// Subtract the entry at index from the sum
	filter->buffer[filter->index] = acc;			// Add the next entry to the buffer
	filter->sum += filter->buffer[filter->index];			// Add the next buffer entry to the sum
	filter->index++;																	// Increment the index, if necessary wrap-around
	if (filter->index == D)
	{
		filter->index = 0;
	}
	filter->averageValue = filter->sum / D;						// Computer average value, we technically could only use sum or average value and not have both
}

/**
 * @brief Initializes a movingAverageFilter.
 * Fills the buffer with zeroes and sets the sum, average
 * and index to zero. 
 * @param filter: The moving average filter to be initialized.
 */
void initializeAccFilter(accFilter *filter) {
	
	
	int i;
	for (i = 0; i < D; i++) {
		filter->buffer[i] = 0.0;
	}
	filter->sum = 0.0f;
	filter->index = 0;
}
