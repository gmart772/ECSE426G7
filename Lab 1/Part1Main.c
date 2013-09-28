	#include <stdio.h>
	#include "STM32F4xx.h" 

	#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
	#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
	#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))

	#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
	#define TRCENA          0x01000000
	
	struct __FILE { int handle; /* Add whatever is needed */ };
		FILE __stdout;
		FILE __stdin;

		int fputc(int ch, FILE *f) {
			if (DEMCR & TRCENA) {
				while (ITM_Port32(0) == 0);
				ITM_Port8(0) = ch;
			}
			return(ch);
		}

	extern void encryptionAsm(unsigned int *key,  char *data, int delta);
	extern void decryptionAsm(unsigned int *key,  char *data, int delta, int sum);
	void encryptionC(unsigned int *key,  char *data, int delta);

	int main(int argc, char* argv[]) {
		
		unsigned int key_temp[4] = { 0x1, 0x2, 0x3, 0x4 };
		char data_temp[8];
		
		int delta 	= 0x9E3779B9;
		int sum 		= 0xC6EF3720;
		
		int i 			= 0;
		
		data_temp[0] = 'A';
		data_temp[1] = 'B';		
		data_temp[2] = 'C';
		data_temp[3] = 'D';
		data_temp[4] = 'E';	
		data_temp[5] = 'F';		
		data_temp[6] = 'G';
		data_temp[7] = 'H';	

		printf("Data_temp: %s\n", data_temp);
		
		encryptionAsm(key_temp, data_temp, delta);
		
		printf("Data_temp: %s\n", data_temp);
		
		decryptionAsm(key_temp, data_temp, delta, sum);
		
		printf("Data_temp: %s\n", data_temp);
		
		return 0;
	}
	
	void encryptionC(unsigned int *key,  char *data, int delta) {
		
	}
