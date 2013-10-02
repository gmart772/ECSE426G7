	#include <stdio.h>
	#include "STM32F4xx.h" 
	
	#include "EncrypteMessages.h"

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
		
	void decryptTAsMessage(); 

	int main(int argc, char* argv[]) {
		unsigned int key_perm[4] = { 3, 5, 9, 4 };
		
		int delta = 0x9E3779B9;
		int sum = 0xC6EF3720;
			
			decryptTAsMessage();
		
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
	
	void decryptTAsMessage() {
		
		int delta = 0x9E3779B9;
		int sum = 0xC6Ef3720;
		int i = 0, k = 0, j = 0;
		unsigned int dataLength = 13;
		//char *data_holder;
		unsigned int decrypt_key[4];
		unsigned int character;
		
		unsigned int data_holder[26];
		unsigned int TA[2] = {'T', 'A'};
		
		unsigned int key[4] = { 3, 5, 9, 4 };
		
		// Ecrypt the letter TA and place them in key 0 and key 1
		encryptionAsm(key, (char *) TA, delta);
		
		decrypt_key[0] = TA[0];
		decrypt_key[1] = TA[1];
		
			for (i = 0x64; i < 0x69; i++)
			{
				for (k = 0x64; k < 0x69; k++)
				{
					decrypt_key[2] = i;
					decrypt_key[3] = k;
					printf("Key Used: %c %c %d %d\n", (char) decrypt_key[3], (char) decrypt_key[2], decrypt_key[1], decrypt_key[0]);
					
					// Copy secret string into data_holder
					memcpy(data_holder, Secret_Quote_Group_7, 26*4);

					for (j = 0; j < dataLength; j++) {
						sum = 0xC6EF3720;
						decryptionAsm(decrypt_key, (char *)data_holder + j*8, delta, sum);
					}					
					printf("Data_out : ");
					for (j = 0; j < 26; j++)
					{
						
						printf("%c", (char) data_holder[j]);
					}
					printf("\n\n");
				}
			}
		
		
		
		
	}
	
