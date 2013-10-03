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
	; R5: Pointer to Data
	; R6-R8: T0-T3
	; R9-R12: K0-K3
	
encryptionAsm
	
	PUSH {R0} ; Put key pointer on stack
	PUSH {R1} ; Put data pointer on stack
	PUSH {R2} ; Put delta on stack
	PUSH {R3} ; Put sum on stack
	PUSH {R4} 
	PUSH {R5} 
	PUSH {R6} 
	PUSH {R7}
	PUSH {R8}
	PUSH {R9}
	PUSH {R10}
	PUSH {R11}
	PUSH {R12}
	
	 ; PUSH {R1}
	; Copy data pointer to temp register
	MOV R5, R1
	
	; Load key into registers 9 - 12
	LDM R0, {R9 - R12}
	
	; Load data into R0 and R1
	LDM R5, {R0 - R1} 
	
	; Set loop counter and sum to 0
	MOV R3, #0
	MOV R4, #0 
	
	; BEGIN LOOP
EncryptionLoop	
	CMP R4, #32 ; Compare loop counter and 32
	BEQ EndEncryptionLoop ; if equal, loop is done, exit
	
	; Sum += Delta
	ADD R3, R3, R2
	
	; T1 = D1 << 4 + K0
	LSL R6, R1, #4
	ADD R6, R6, R9
	
	; T2 = D1 >> 5 + K1
	LSR R7, R1, #5
	ADD R7, R7, R10
	
	; T3 = D1 + Sum
	ADD R8, R1, R3
	
	; T4 = T1 XOR T1 XOR T3
	EOR R6, R6, R7
	EOR R6, R6, R8
	
	; D0 = D0 + T4
	ADD R0, R0, R6
	
	; T1 = D0 << 4 + K2
	LSL R6, R0, #4
	ADD R6, R6, R11
	
	; T2 = D0 >> 5 + K3
	LSR R7, R0, #5
	ADD R7, R7, R12
	
	; T3 = D0 + Sum
	ADD R8, R0, R3
	
	; T4 = T1 XOR T2 XOR T3
	EOR R6, R6, R7
	EOR R6, R6, R8
	
	; D1 = D1 + T4
	ADD R1, R1, R6
	
	ADD R4, R4, #1 ; increment counter
	B	EncryptionLoop
EndEncryptionLoop	

	; AFTER LOOP
	STM R5!, {R0 - R1} ; Put data back in memory

	POP {R12}
	POP {R11}
	POP {R10}
	POP {R9}
	POP {R8}
	POP {R7}
	POP {R6}
	POP {R5}
	POP {R4}
	POP {R3}
	POP {R2}
	POP {R1}
	POP {R0}
	; Increment stack pointer by 3 registers
	
	BX LR

	ENDP
	
decryptionAsm 
	PUSH {R0} ; Put key pointer on stack
	PUSH {R1} ; Put data pointer on stack
	PUSH {R2} ; Put delta on stack
	PUSH {R3} ; Put sum on stack
	PUSH {R4} 
	PUSH {R5}
	PUSH {R6}
	PUSH {R7}
	PUSH {R8}
	PUSH {R9}
	PUSH {R10}
	PUSH {R11}
	PUSH {R12}
	
	; Copy data pointer to temp register
	MOV R5, R1 
	
	; Load key into registers 9 - 12
	LDM R0, {R9 - R12}
	
	; Load data into R0 and R1
	LDM R5, {R0 - R1} 
	
	; Set loop counter to 0
	MOV R4, #0 
	
	; BEGIN LOOP
DecryptionLoop	
	CMP R4, #32 ; Compare loop counter and 32
	BEQ EndDecryptionLoop ; if equal, loop is done, exit
	
	; T1 = D0 << 4 + K2
	LSL R6, R0, #4
	ADD R6, R6, R11
	
	; T2 = D0 >> 5 + K3
	LSR R7, R0, #5
	ADD R7, R7, R12
	
	; T3 = D0 + Sum
	ADD R8, R0, R3
	
	; T4 = T1 XOR T1 XOR T3
	EOR R6, R6, R7
	EOR R6, R6, R8
	
	; D1 = D1 + T4
	SUB R1, R1, R6
	
	; T1 = D1 << 4 + K0
	LSL R6, R1, #4
	ADD R6, R6, R9
	
	; T2 = D1 >> 5 + K1
	LSR R7, R1, #5
	ADD R7, R7, R10
	
	; T3 = D1 + Sum
	ADD R8, R1, R3
	
	; T4 = T1 XOR T1 XOR T3
	EOR R6, R6, R7
	EOR R6, R6, R8
	
	; D0 = D0 - T4
	SUB R0, R0, R6
	
	; increment counter
	ADD R4, R4, #1 
	
	; Sum -= Delta
	SUB R3, R3, R2
	
	B	DecryptionLoop
EndDecryptionLoop	

	; AFTER LOOP
	STM R5!, {R0 - R1} ; Put data back in memory
	
	POP {R12}
	POP {R11}
	POP {R10}
	POP {R9}
	POP {R8}
	POP {R7}
	POP {R6}
	POP {R5}
	POP {R4}
	POP {R3}
	POP {R2}
	POP {R1}
	POP {R0}
	
	; Increment stack pointer by 3 registers
	
	BX LR

	ENDP
	
	END