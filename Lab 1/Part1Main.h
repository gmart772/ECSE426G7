	
	
	#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
	#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
	#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))

	#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
	#define TRCENA          0x01000000
	
	
	
	extern void encryptionAsm(unsigned int *key,  char *data, int delta);
	
	extern void decryptionAsm(unsigned int *key,  char *data, int delta, int sum);
	
	void encryptionC(unsigned int *key,  char *data, int delta);
	
	void decryptionC(unsigned int *key,  char *data, int delta, int sum); 
	
	// Decrypts an array of unit32_t of size 26 using key { 3, 5, 9, 4 }
	void decryptMessage(uint32_t *data); 