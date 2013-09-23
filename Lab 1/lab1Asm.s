	AREA Lab1Asm, CODE, READONLY

	EXPORT encryptionAsm
	
encryptionAsm	

	MOV R1, #25
	MOV R2, #75
	ADD R1, R1, R2

	BX LR;

	END