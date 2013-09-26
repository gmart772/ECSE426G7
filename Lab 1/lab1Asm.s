	AREA Lab1Asm, CODE, READONLY

	EXPORT encryptionAsm
	EXPORT decryptionAsm
	; Lab 1, Assembly encryption/decryption routines
	; REGISTERS
	; R0: Pointer to key -> D0
	; R1: Pointer to data -> D1
	; R2: Delta
	; R3: Sum
	; R4: Loop counter (N)
	; R5: Temp
	; R6-R8: T0-T3
	; R9-R12: K0-K3
	
encryptionAsm		
	
	PUSH {R0} ; Put key pointer on stack
	PUSH {R1} ; Put data pointer on stack
	PUSH {R2} ; Put delta on stack
	
	MOV R5, R1 ; Copy data pointer to temp register
	
	LDM R0, {R9 - R12} ; Load key into registers 9 - 12
	
	LDM R5, {R0 - R1} ; Load data into D0 and D1
	
	MOV R4, #0 ; Set loop counter to 0
	
	MOV R0, #0xCAFE
	
	STM R5, {R0 - R1} ; Put data back in memory
	
	;POP {R2}
	;POP {R1}
	;POP {R0}
	; Increment stack pointer by 3 registers
	
	BX LR

	ENDP
	
decryptionAsm 

	END