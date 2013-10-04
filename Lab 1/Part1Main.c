	#include <stdio.h>
	#include "STM32F4xx.h" 
	#include "Part1Main.h"
	#include "EncrypteMessages.h"
	
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

	int main(int argc, char* argv[]) {
		
		int delta = 0x9E3779B9;
		int sum = 0xC6Ef3720;
		
		unsigned int key_perm[4] = { 3, 5, 9, 4 };
		
		char *star_trek = "Space: the final frontier. These are the voyages of the starship Enterprise. Its continuing mission to explore strange new worlds, to seek out new life and new civilizations, to boldly go where no one has gone before.       ";
		int j = 0;
		unsigned int data_holder[56];
		
		/* TEST ENCRYPTION AND DECRYPTION SUB ROUTINES AND TIMES */
		char test_data[8] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' };
		printf("Data is %s to start\n\n", test_data);
		// Assembly only
		encryptionAsm(key_perm, test_data, delta);
		printf("Data is now %s\n", test_data);
		decryptionAsm(key_perm, test_data, delta, sum);	
		printf("Data is now %s\n\n", test_data);
		// C only
		encryptionC(key_perm, test_data, delta);
		printf("Data is now %s\n", test_data);
		decryptionC(key_perm, test_data, delta, sum);	
		printf("Data is now %s\n\n", test_data);
		// Encrypt ASM Decrypt C
		encryptionAsm(key_perm, test_data, delta);
		printf("Data is now %s\n", test_data);
		decryptionC(key_perm, test_data, delta, sum);	
		printf("Data is now %s\n\n", test_data);
		// Encrypt C Decrypt ASM
		encryptionC(key_perm, test_data, delta);
		printf("Data is now %s\n", test_data);
		decryptionAsm(key_perm, test_data, delta, sum);	
		printf("Data is now %s\n\n", test_data);	
		printf("\n");
		printf("\n");	
		

		// Encrypt star_trek string
		memcpy(data_holder, star_trek, 28*8);					// Copy star_trek string into our data_holder
		for (j = 0; j < 28; j++) {
			encryptionC(key_perm, (char *)data_holder + j*8, delta);			// Encrypt string 8 characters at a time
		}
		// Decrypt star_trek string
		for (j = 0; j < 28; j++) {
			decryptionC(key_perm, (char *) data_holder + j*8, delta, sum);	// Decrypt string 8 characters at a time
		}
		
		// Decrypt the Secret Quote for group 7 (key is hardcoded inside to {3, 5, 9, 4}
		decryptMessage(Secret_Quote_Group_7);
		
			return 0;
	}
	
	void encryptionC(unsigned int *key,  char *data, int delta) {
		// Declare variables that will be used for encryption loop
		unsigned int sum = 0;
		int N;										// Loop counter
		unsigned int t1, t2, t3; 	// temp sums
		unsigned int *d0, *d1;		// Pointer to data
		
		// Assign data to the pointers
		d0 = (unsigned int *) &data[0];
		d1 = (unsigned int *) &data[4];			
		
		// encryption loop algorithm
		for (N = 0; N < 32; N++) {
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
		int N;										// Loop counter
		unsigned int t1, t2, t3; 	// temp sums
		unsigned int *d0, *d1;		// Pointer to data
		
		// Assign data to the pointers
		d0 = (unsigned int *) &data[0];
		d1 = (unsigned int *) &data[4];
		
		// Decryption loop algorithm
		for (N = 0; N < 32; N++) {
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
	
	void decryptMessage(uint32_t *data) {
		
		// Initalize variables
		int delta = 0x9E3779B9;					// Delta for encrypt/decryot
		int sum = 0xC6Ef3720;						// Sum for decryption algorithm
		int i = 0, k = 0, j = 0;				// Variables that will be used for loops
		unsigned int dataLength = 13;		// Data length is set to 13 (13 x (2 x uint32_t) of data in secret quote)
		unsigned int decrypt_key[4];		// Decryption key that will be used
		
		
		unsigned int data_holder[26];		// Variable that will contain each decrypted message
		unsigned int TA[2] = {'T', 'A'};// Integer array for TA that will be encrypted
		
		unsigned int key[4] = { 3, 5, 9, 4 };	// Key used to encrypt TA
		
		// Ecrypt the letter TA and place them in key 0 and key 1
		encryptionAsm(key, (char *) TA, delta);	
		decrypt_key[0] = TA[0];
		decrypt_key[1] = TA[1];
		
		// Loop through all possible keys and decrypt the secret quote using it
			for (i = 0x64; i < 0x69; i++)
			{
				decrypt_key[2] = i;	// Assign decrypt_key[2] to the next value
				
				for (k = 0x64; k < 0x69; k++)
				{
					decrypt_key[3] = k;	// Assign decrypt_key[3] to the next value				
					printf("Key Used: %c %c %d %d\n", (char) decrypt_key[3], (char) decrypt_key[2], decrypt_key[1], decrypt_key[0]); // Print key used (cast [3] and [2] to characters)
					
					// Copy secret string into data_holder
					memcpy(data_holder, data, 26*4);

					// Loop through entire length of Secret_Quote_Group_7 and decrypt it back into data_holder
					for (j = 0; j < dataLength; j++) {
						decryptionAsm(decrypt_key, (char *)data_holder + j*8, delta, sum);
					}					
					
					// Print the current decrypted data
					printf("Data_out : ");
					for (j = 0; j < 26; j++)
					{		
						printf("%c", (char) data_holder[j]);
					}
					printf("\n\n");
				}
			}
	}
