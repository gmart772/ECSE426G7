#include <stdio.h>
#include "stm32f4xx.h"

static volatile uint_fast16_t ticks;

void timer(){
ticks = 0;
// Configured for 50 ms period
// Configure SysTick to be 20Hz
// NOTE: argument here must be less than 0xFFFFFF; //(24 bit timer)
SysTick_Config(SystemCoreClock/20); // Number of ticks between two interrupts or SystemCoreClock/Freq
	
	while(1){
		
		// Wait for an interrupt
		while(!ticks);
		
		// Decrement ticks
		ticks = 0;
		
		// Interrupt routine
		
		
	}
}


//Interrupt handler for system tick
//This should happen every 50 ms
void SysTick_Handler()
{
	ticks = 1;
}