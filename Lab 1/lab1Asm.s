	AREA Lab1Asm, CODE, READONLY

	EXPORT encryptionAsm
	EXPORT decryptionAsm
	; Lab 1, Assembly encryption/decryption routines
	; REGISTERS
	; R0: Pointer to key -> D0
	; R1: Pointer to data -> D1
	; R2: Sum
	; R3: Delta
	; R4: Loop counter (N)
	; R5: Temp
	; R6-R8: T0-T3
	; R9-R12: K0-K3
	
encryptionAsm	
	
	POP {R0} ; Get pointer to key from stack
	POP {R1} ; Get pointer to data from stack
	
	MOV R5, R1 ; Copy data pointer to temp register
	
	LDM R0, {R9-R12} ; Load key into registers 9 - 12
	
	LDM R5, {R0 - R1} ; Load data into D0 and D1
	
	BX LR

	ENDP
	
decryptionAsm 

	END