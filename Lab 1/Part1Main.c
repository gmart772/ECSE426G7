	#include <stdio.h>
	#include "STM32F4xx.h" 

	extern void encryptionAsm(unsigned int *key,  char *data, int delta);

	int __main(int argc, char* argv[]) {
//		printf("test\n");
		//int x = 1;
		//int y = 2;
		//int z = x + y;
		unsigned int key_temp[4];
		char *data_temp;
		int delta = 0x9E3779B9;
		
		//key_temp = { 0xBEEF, 0xDEAD, 0xCAFE, 0xFADE };
		int i = 0;
		for (i = 0; i < 4; i++) {
			key_temp[i] = 0xBEEF;
		}
		
		data_temp = "TESTTEST";
		
	//	for (i = 0; i < 4; i++) {
	//		data_temp[i] = 0x00;
	//	}
		

		
		encryptionAsm(key_temp, data_temp, delta);
		
		printf("data_temp: %s", data_temp);
		
		return 0;
	}
