[bits 32]					; 32비트 모드

[extern ClcMain]			; void main();

segment .text
	[global KernelStartup]
	KernelStartup:			; void KernelStartup();
		call ClcMain		; ClcMain();			/* ClcMain함수 호출 */
		jmp $				; while(1);				/* 멈춤 */

	; 특수 load 어셈블리어
	[global LoadGdt]		; void LoadGdt(WORD size, Gdt *address);
	LoadGdt:
		mov ax, [esp + 4]
		mov [esp + 6], ax
		lgdt [esp + 6]
		ret

	[global LoadIdt]		; void LoadIdt(WORD size, Idt *address);
	LoadIdt:
		mov ax, [esp + 4]
		mov [esp + 6], ax
		lidt [esp + 6]
		ret

	[global LoadTr]			; void LoadTr(DWORD tr);
	LoadTr:
		ltr [esp + 4]
		ret

	; In/Out 명령들
	[global InByte]			; BYTE InByte(BYTE port);
	InByte:
		mov edx, [esp + 4]
		mov eax, 0
		in al, dx
		ret
	[global InWord]			; WORD InWord(BYTE port);
	InWord:
		mov edx, [esp + 4]
		mov eax, 0
		in ax, dx
		ret
	[global OutByte]		; void OutByte(BYTE port, BYTE data);
	OutByte:
		mov edx, [esp + 4]
		mov eax, [esp + 8]
		out dx, al
		ret
	[global OutWord]		; void OutWord(BYTE port, WORD data);
	OutWord:
		mov edx, [esp + 4]
		mov eax, [esp + 8]
		out dx, ax
		ret

	; 어셈블리 함수
	[global memcpy]			; void memcpy(void *dest, const void *src, unsigned size);
	memcpy:
		push edi
		push esi
		push ecx

		mov edi, [esp + 12 + 4]
		mov esi, [esp + 12 + 8]
		mov ecx, [esp + 12 + 12]

		cld
		rep movsb

		pop ecx
		pop esi
		pop edi
		ret

	[global memset]			; void memset(void *mem, BYTE c, unsigned size);
	memset:
		push edi
		push ecx

		mov edi, [esp + 8 + 4]
		mov eax, [esp + 8 + 8]
		mov ecx, [esp + 8 + 12]

		cld
		rep stosb

		pop ecx
		pop edi
		ret

	[global memset_2]		; void memset_2(void *mem, WORD s, unsigned size);
	memset_2:
		push edi
		push ecx

		mov edi, [esp + 8 + 4]
		mov eax, [esp + 8 + 8]
		mov ecx, [esp + 8 + 12]
		shr ecx, 1

		cld
		rep stosw

		pop ecx
		pop edi
		ret

	[global memset_4]		; void memset_4(void *mem, DWORD i, unsigned size);
	memset_4:
		push edi
		push ecx

		mov edi, [esp + 8 + 4]
		mov eax, [esp + 8 + 8]
		mov ecx, [esp + 8 + 12]
		shr ecx, 2

		cld
		rep stosd

		pop ecx
		pop edi
		ret

	[global memchr_4]		; void *memchr_4(void *mem, DWORD i, unsigned size);
	memchr_4:
		push edi
		push ecx

		mov edi, [esp + 8 + 4]
		mov eax, [esp + 8 + 8]
		mov ecx, [esp + 8 + 12]
		shr ecx, 2

		cld
		repne cmpsd
		mov eax, edi

		pop ecx
		pop edi
		ret

	[global GetTimeStamp]	; void GetTimeStamp(DWORD *high, DWORD *low); // Timer.h
	GetTimeStamp:
		rdtsc
		mov [esp + 4], edx
		mov [esp + 8], eax
		ret

	; 기타 어셈블리 명령들
	[global AsmCli]			; void AsmCli();
	AsmCli:
		cli
		ret
	[global AsmSti]			; void AsmSti();
	AsmSti:
		sti
		ret
	[global AsmHlt]			; void AsmHlt();
	AsmHlt:
		hlt
		ret
	[global AsmStiHlt]			; void AsmStiHlt();
	AsmStiHlt:
		sti
		hlt
		ret

	[global GetEFlag]			; DWORD GetEFlag();
	GetEFlag:
		pushfd
		pop eax
		ret
	[global SetEFlag]			; void SetEFlag(DWORD eflag);
	SetEFlag:
		push dword [esp + 4]
		popfd
		ret

	[global GetCr0]			; DWORD GetCr0();
	GetCr0:
		mov eax, cr0
		ret
	[global SetCr0]			; void SetCr0(DWORD cr0);
	SetCr0:
		mov eax, dword [esp + 4]
		mov cr0, eax
		ret

	[global FarJmp]			; void FarJmp(DWORD eip, WORD cs);
	FarJmp:
		jmp far [esp + 4]
		ret

; Interrupt Handlers
%macro PushRegisters 0
        push eax
        push ebx
        push ecx
        push edx
        push esi
        push edi
        push ebp
        push ds
        push es
        push fs
        push gs

        mov ax, 2 * 8
        mov ds, ax
        mov es, ax
        mov gs, ax
        mov fs, ax
%endmacro
%macro PopRegisters 0
        pop gs
        pop fs
        pop es
        pop ds
        pop ebp
        pop edi
        pop esi
        pop edx
        pop ecx
        pop ebx
        pop eax
%endmacro

%macro MakeIntHandler 1
	[global %1]
	[extern _%1]
	%1:
		PushRegisters

		push esp
		call _%1
		add esp, 4

		PopRegisters
		iretd
%endmacro

%macro DefaultHandler 1
    [extern _DefaultHandler]
    [global DefaultHandler%1]
    DefaultHandler%1:
        PushRegisters

        push esp
        push %1
        call _DefaultHandler
        add esp, 8

        or eax, eax
        jz .skip
        add esp, 4
    .skip:
        PopRegisters
        iretd
%endmacro

DefaultHandler 0
DefaultHandler 1
DefaultHandler 2
DefaultHandler 3
DefaultHandler 4
DefaultHandler 5
DefaultHandler 6
DefaultHandler 7
DefaultHandler 8
DefaultHandler 9
DefaultHandler 10
DefaultHandler 11
DefaultHandler 12
DefaultHandler 13
DefaultHandler 14
DefaultHandler 15
DefaultHandler 16
DefaultHandler 17
DefaultHandler 18
DefaultHandler 19

MakeIntHandler TimerHandler
MakeIntHandler KeyboardHandler
MakeIntHandler MouseHandler
