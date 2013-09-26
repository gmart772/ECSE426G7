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

	extern char encryptionAsm(unsigned int *key,  char *data, int delta);

	int __main(int argc, char* argv[]) {
//		printf("test\n");
		//int x = 1;
		//int y = 2;
		//int z = x + y;
		unsigned int key_temp[4];
		char data_temp[8];
		
		int delta = 0x9E3779B9;
		
		//key_temp = { 0xBEEF, 0xDEAD, 0xCAFE, 0xFADE };
		int i = 0;
		for (i = 0; i < 4; i++) {
			key_temp[i] = 0xDEADBEEF;
		}
		
	//	data_temp = " 0@P 0@P";
			data_temp[0] = 'A';
			data_temp[1] = 'B';		
			data_temp[2] = 'C';
			data_temp[3] = 'D';
			data_temp[4] = 'E';	
			data_temp[5] = 'F';		
			data_temp[6] = 'G';
			data_temp[7] = 'H';		
			
	//	for (i = 0; i < 4; i++) {
	//		data_temp[i] = 0x00;
	//	}
		

		
		encryptionAsm(key_temp, data_temp, delta);
		
		printf("Data_temp: %s", data_temp);
		
		return 0;
	}
