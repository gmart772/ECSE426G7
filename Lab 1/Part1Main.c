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
		
	void decryptTAsMessage(unsigned int *key,  char *data); 

	int main(int argc, char* argv[]) {
		unsigned int key_perm[4] = { 0x1, 0x2, 0x3, 0x4 };
		char data_temp[8];

		int delta = 0x9E3779B9;
		int sum = 0xC6EF3720;

			data_temp[0] = 'A';
			data_temp[1] = 'T';		
			data_temp[2] = ' ';
			data_temp[3] = ' ';
			data_temp[4] = ' ';	
			data_temp[5] = ' ';		
			data_temp[6] = ' ';
			data_temp[7] = ' ';

			printf("Data_temp pre-encryption: %s\n", data_temp);
			encryptionC(key_perm, data_temp, delta);	
			printf("Data_temp encrypted: %s\n", data_temp);	
			decryptionC(key_perm, data_temp, delta, sum);		
			printf("Data_temp decrypted: %s\n", data_temp);
			printf("Data_temp pre-encryption: %s\n", data_temp);
			encryptionAsm(key_perm, data_temp, delta);
			printf("Data_temp encrypted: %s\n", data_temp);		
			decryptionAsm(key_perm, data_temp, delta, sum);	
			printf("Data_temp decrypted: %s\n", data_temp);
			
			decryptTAsMessage( key_perm, (char *)Secret_Quote_Group_7);
		
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
	
	void decryptTAsMessage(unsigned int *key, char *data) {
		
		int delta = 0x9E3779B9;
		int sum = 0xC6Ef3720;
		int i = 0, k = 0;
		
		// Create a new key for decryption
		unsigned int decrypt_key[4];
		
		// Data used to generate first two 32-bit keys
		char TAData[8] = { ' ', ' ', ' ', ' ', ' ', ' ', 'T', 'A' };
		
		// Ecrypt the letter TA and place them in key 0 and key 1
		encryptionC(key, TAData, delta);
		decrypt_key[0] = (unsigned int) TAData[0];
		decrypt_key[1] = (unsigned int) TAData[4];
		
		for (i = 0; i < 4; i++)
		{
			for (k = 0; k < 4; k++)
			{
				switch (i)
				{
					case 0 ://d :
						
						switch (k)
						{
							case 0 ://d :
								decrypt_key[2] = 0x64;
								decrypt_key[3] = 0x64;
								decryptionC(decrypt_key, data, delta, sum);
								printf("Key Used: %s\n", (char *)decrypt_key);
								printf("Decrypted Message: %s\n", data);
								printf("\n");
							case 1 : //e :
								decrypt_key[2] = 0x64;
								decrypt_key[3] = 0x65;
								decryptionC(decrypt_key, data, delta, sum);
								printf("Key Used: %s\n", (char *)decrypt_key);
								printf("Decrypted Message: %s\n", data);
								printf("\n");
							case 2 : //f :
								decrypt_key[2] = 0x64;
								decrypt_key[3] = 0x66;
								decryptionC(decrypt_key, data, delta, sum);
								printf("Key Used: %s\n", (char *)decrypt_key);
								printf("Decrypted Message: %s\n", data);
							printf("\n");
							case 3 : //g :
								decrypt_key[2] = 0x64;
								decrypt_key[3] = 0x67;
								decryptionC(decrypt_key, data, delta, sum);
								printf("Key Used: %s\n", (char *)decrypt_key);
								printf("Decrypted Message: %s\n", data);
								printf("\n");
							case 4 : //h :
								decrypt_key[2] = 0x64;
								decrypt_key[3] = 0x68;
								decryptionC(decrypt_key, data, delta, sum);
								printf("Key Used: %s\n", (char *)decrypt_key);
								printf("Decrypted Message: %s\n", data);
								printf("\n");
							default :
								break;
						}	
					
					case 1 : //e :
						switch (k)
						{
							case 0 : //d :
								decrypt_key[2] = 0x65;
								decrypt_key[3] = 0x64;
								decryptionC(decrypt_key, data, delta, sum);
								printf("Key Used: %s\n", (char *)decrypt_key);
								printf("Decrypted Message: %s\n", data);
								printf("\n");
							case 1 : //e :
								decrypt_key[2] = 0x65;
								decrypt_key[3] = 0x65;
								decryptionC(decrypt_key, data, delta, sum);
								printf("Key Used: %s\n", (char *)decrypt_key);
								printf("Decrypted Message: %s\n", data);
								printf("\n");
							case 2 : //f :
								decrypt_key[2] = 0x65;
								decrypt_key[3] = 0x66;
								decryptionC(decrypt_key, data, delta, sum);
								printf("Key Used: %s\n", (char *)decrypt_key);
								printf("Decrypted Message: %s\n", data);
								printf("\n");
							case 3 : //g :
								decrypt_key[2] = 0x65;
								decrypt_key[3] = 0x67;
								decryptionC(decrypt_key, data, delta, sum);
								printf("Key Used: %s\n", (char *)decrypt_key);
								printf("Decrypted Message: %s\n", data);
								printf("\n");
							case 4 : //h :
								decrypt_key[2] = 0x65;
								decrypt_key[3] = 0x68;
								decryptionC(decrypt_key, data, delta, sum);
								printf("Key Used: %s\n", (char *)decrypt_key);
								printf("Decrypted Message: %s\n", data);
								printf("\n");
							default :
								break;
						}	
						
					case 2 : //f :
						
						switch (k)
						{
							case 0 : //d :
								decrypt_key[2] = 0x66;
								decrypt_key[3] = 0x64;
								decryptionC(decrypt_key, data, delta, sum);
								printf("Key Used: %s\n", (char *)decrypt_key);
								printf("Decrypted Message: %s\n", data);
								printf("\n");
							case 1 : //e :
								decrypt_key[2] = 0x66;
								decrypt_key[3] = 0x65;
								decryptionC(decrypt_key, data, delta, sum);
								printf("Key Used: %s\n", (char *)decrypt_key);
								printf("Decrypted Message: %s\n", data);
								printf("\n");
							case 2 : //f :
								decrypt_key[2] = 0x66;
								decrypt_key[3] = 0x66;
								decryptionC(decrypt_key, data, delta, sum);
								printf("Key Used: %s\n", (char *)decrypt_key);
								printf("Decrypted Message: %s\n", data);
								printf("\n");
							case 3 : //g :
								decrypt_key[2] = 0x66;
								decrypt_key[3] = 0x67;
								decryptionC(decrypt_key, data, delta, sum);
								printf("Key Used: %s\n", (char *)decrypt_key);
								printf("Decrypted Message: %s\n", data);
								printf("\n");
							case 4 : //h :
								decrypt_key[2] = 0x66;
								decrypt_key[3] = 0x68;
								decryptionC(decrypt_key, data, delta, sum);
								printf("Key Used: %s\n", (char *)decrypt_key);
								printf("Decrypted Message: %s\n", data);
								printf("\n");
							default :
						}	
						
					case 3 : //g :
						
						switch (k)
						{
							case 0 : //d :
								decrypt_key[2] = 0x67;
								decrypt_key[3] = 0x64;
								decryptionC(decrypt_key, data, delta, sum);
								printf("Key Used: %s\n", (char *)decrypt_key);
								printf("Decrypted Message: %s\n", data);
								printf("\n");
							case 1 : //e :
								decrypt_key[2] = 0x67;
								decrypt_key[3] = 0x65;
								decryptionC(decrypt_key, data, delta, sum);
								printf("Key Used: %s\n", (char *)decrypt_key);
								printf("Decrypted Message: %s\n", data);
								printf("\n");
							case 2 : //f :
								decrypt_key[2] = 0x67;
								decrypt_key[3] = 0x66;
								decryptionC(decrypt_key, data, delta, sum);
								printf("Key Used: %s\n", (char *)decrypt_key);
								printf("Decrypted Message: %s\n", data);
								printf("\n");
							case 3 : //g :
								decrypt_key[2] = 0x67;
								decrypt_key[3] = 0x67;
								decryptionC(decrypt_key, data, delta, sum);
								printf("Key Used: %s\n", (char *)decrypt_key);
								printf("Decrypted Message: %s\n", data);
								printf("\n");
							case 4 : //h :
								decrypt_key[2] = 0x67;
								decrypt_key[3] = 0x68;
								decryptionC(decrypt_key, data, delta, sum);
								printf("Key Used: %s\n", (char *)decrypt_key);
								printf("Decrypted Message: %s\n", data);
								printf("\n");
							default :
								break;
						}	
						
					case 4 : //h :
						
						switch (k)
						{
							case 0 : //d :
								decrypt_key[2] = 0x68;
								decrypt_key[3] = 0x64;
								decryptionC(decrypt_key, data, delta, sum);
								printf("Key Used: %s\n", (char *)decrypt_key);
								printf("Decrypted Message: %s\n", data);
								printf("\n");
							case 1 : //e :
								decrypt_key[2] = 0x68;
								decrypt_key[3] = 0x65;
								decryptionC(decrypt_key, data, delta, sum);
								printf("Key Used: %s\n", (char *)decrypt_key);
								printf("Decrypted Message: %s\n", data);
								printf("\n");
							case 2 : //f :
								decrypt_key[2] = 0x68;
								decrypt_key[3] = 0x66;
								decryptionC(decrypt_key, data, delta, sum);
								printf("Key Used: %s\n", (char *)decrypt_key);
								printf("Decrypted Message: %s\n", data);
								printf("\n");
							case 3 : //g :
								decrypt_key[2] = 0x68;
								decrypt_key[3] = 0x67;
								decryptionC(decrypt_key, data, delta, sum);
								printf("Key Used: %s\n", (char *)decrypt_key);
								printf("Decrypted Message: %s\n", data);
								printf("\n");
							case 4 : //h :
								decrypt_key[2] = 0x68;
								decrypt_key[3] = 0x68;
								decryptionC(decrypt_key, data, delta, sum);
								printf("Key Used: %s\n", (char *)decrypt_key);
								printf("Decrypted Message: %s\n", data);
								printf("\n");
							default :
								break;
						}				
					default :
						break;
				}	
			}
		}
		
		
		
		
	}
	
