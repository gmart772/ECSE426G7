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
	void decryptionC(unsigned int *key,  char *data, int delta, int sum); 

	int main(int argc, char* argv[]) {
		
		unsigned int key_temp[4] = { 0x1, 0x2, 0x3, 0x4 };
		char data_temp[8];

		int delta = 0x9E3779B9;
		int sum = 0xC6EF3720;
		//key_temp = { 0xBEEF, 0xDEAD, 0xCAFE, 0xFADE };
		int i = 0;
		for (i = 0; i < 4; i++) {
			key_temp[i] = i + 1;
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

			printf("Data_temp pre-encryption: %s\n", data_temp);
			

			encryptionC(key_temp, data_temp, delta);

		
			printf("Data_temp encrypted: %s\n", data_temp);
		
			decryptionC(key_temp, data_temp, delta, sum);
		
			printf("Data_temp decrypted: %s\n", data_temp);

			printf("Data_temp pre-encryption: %s\n", data_temp);
			

			encryptionAsm(key_temp, data_temp, delta);

		
			printf("Data_temp encrypted: %s\n", data_temp);
		
			decryptionAsm(key_temp, data_temp, delta, sum);
		
			printf("Data_temp decrypted: %s\n", data_temp);
		
			return 0;
	}
	
	void encryptionC(unsigned int *key,  char *data, int delta) {
		unsigned int sum = 0;
		int i, N;
		unsigned int t1, t2, t3; // temp sums
		int *d0, *d1;
		
		d0 = (int *) &data[0];
		d1 = (int *) &data[4];		
		
		
		N = 32; 
		
		// initialize variables
		//t1 = t2 = t3 = 0;
		
		// get data values
	
		// encryption loop
		for (i = 0; i < N; i++) {
			sum += delta;
			
			t1 = (*d1 << 4) + key[0];
			t2 = (*d1 >> 5) + key[1];
			t3 = *d1 + sum;
			t3 = t1 ^ t2 ^ t3;
			*d0 += t3;
			
			t1 = (*d0 << 4) + key[2];
			t2 = (*d0 >> 5) + key[3];
			t3 = *d0 + sum;
			t3 = t1 ^ t2 ^ t3;
			*d1 += t3;

		}
	}
	
	void decryptionC(unsigned int *key,  char *data, int delta, int sum) {
		int i;
		unsigned int t1, t2, t3; // temp sums
		int *d0, *d1;
		
		d0 = (int *) &data[0];
		d1 = (int *) &data[4];	
		
		for (i = 0; i < 32; i++) {
			t1 = (*d0 << 4) + key[2];
			t2 = (*d0 >> 5) + key[3];
			t3 = *d0 + sum;
			t3 = t1 ^ t2 ^ t3;
			*d1 -= t3;
			
			t1 = (*d1 << 4) + key[0];
			t2 = (*d1 >> 5) + key[1];
			t3 = *d1 + sum;
			t3 = t1 ^ t2 ^ t3;
			*d0 -= t3;
			
			sum -= delta;
		}
	}
