%include "../Bootsector/init.inc"

; VBE
VBE_INFO_SEGMENT equ 0x0200						; VBE�� ������ ����Ǵ� ���׸�Ʈ�� ��ġ
VBE_MODE equ 0x117								; VBE ȭ�� ���

; BootInfo
BOOT_INFO equ 0x7000							; BootInfo�� ��ġ
READ_CYLS equ 0									; �о���� �Ǹ��� ��
SCREEN_X equ 1									; ȭ���� X��ǥ ũ��
SCREEN_Y equ 3									; ȭ���� Y��ǥ ũ��
VIDEO_MEMORY equ 5								; ���� �޸��� ������
BOOT_INFO_SIZE equ 9							; BootInfo�� ũ��

; ����
FLOPPY_CPY_ADD equ 0x100000						; �÷��� ��ũ�� �����Ͱ� ����Ǵ� �ּ�
BOOT_INFO_CPY_ADD equ 0x268000					; BootInfo�� ����Ǵ� �ּ�
KERNEL_CPY_ADD equ 0x500000						; Ŀ���� ����Ǵ� �ּ�
KERNEL_STACK equ 0x500000						; Ŀ�� ������ �ּ�
KERNEL_SIZE equ (1024*540)						; Ŀ���� �ִ� ũ�� = 540KB

[org 0]
[bits 16]

Kernel_start:
	mov ax, cs									; cs�� ds�� ���� �Ѵ�
	mov ds, ax

	mov bx, 0
	mov fs, bx									; fs = 0

	; {{16-bit �ڵ� �ۼ�

	mov byte [fs:BOOT_INFO + READ_CYLS], READ_CYLINDERS ; �� �Ǹ����� �о����� BootInfo�� ���

	mov ax, VBE_INFO_SEGMENT
	mov es, ax
	mov di, 0									; es:di�� VBE������ ����
	mov ax, 0x4f00								; ax=0x4f00 -> VBE���� �� ���� ���� Ȯ��
	int 0x10									; BIOS ȣ��
	cmp ax, 0x004f								; VBE�� �ִٸ� ax�� ���� 0x004f�̴�
	jne vbe_error								; ax�� 0x004f�� �ƴ϶�� vbe_error�� ����

	mov ax, [es:di + 4]							; es:di + 4���� ���� ������ �ִ�.
	cmp ax, 0x0200								; ������ 0x0200����(2.0����) �̻����� Ȯ���Ѵ�
	jb vbe_error								; 0x0200 �̸��̶�� vbe_error�� ����

	jmp vbe_success								; vbe_success�� ����

vbe_error:
	mov si, vbe_error_msg						; ���� �޼����� �ּ�
putloop:
	mov al, [si]								; �޼����� �� ���� ����
	inc si										; si++
	cmp al, 0									; al�� 0�� ��
	je stop										; 0�̸� stop���� jmp
	mov ah, 0x0e								; ah=0x0e -> ȭ�鿡 �� ���� ���
	mov bx, 15									; �� �ڵ�
	int 0x10									; BIOS ȣ��(al�� ���� �ڵ�)
	jmp putloop									; ������ ��
stop:
	hlt											; CPU�� �Ͻ�����
	jmp stop									; ���ѷ���

vbe_error_msg db "Clubcos0 requires VBE 2.0, but this computer doesn't support VBE 2.0. System stop.", 0

vbe_success:
	mov ax, VBE_INFO_SEGMENT
	mov es, ax
	mov di, 0									; es:di�� ȭ�� ��� ������ ����
	mov ax, 0x4f01								; ax=0x4f01 -> ȭ�� ��� ���� ���� �� ���� Ȯ��
	mov cx, VBE_MODE								; cx -> ȭ�� ���
	int 0x10									; BIOS ȣ��
	cmp ax, 0x004f								; �����ϸ� ax�� ���� 0x004f�̴�
	jne vbe_error								; �ƴ϶�� vbe_error�� ����

	mov ax, [es:di + 0x12]						; ȭ�� ���+0x12 -> ȭ�� X���� ũ��(2byte = word)
	mov [fs:BOOT_INFO + SCREEN_X], ax			; BootInfo�� ���
	mov ax, [es:di + 0x14]						; ȭ�� ���+0x14 -> ȭ�� Y���� ũ��(2byte = word)
	mov [fs:BOOT_INFO + SCREEN_Y], ax			; BootInfo�� ���
	mov eax, [es:di + 0x28]						; ȭ�� ��� ����+0x28 -> ���� �޸��� ��ġ(pointer = 4byte = dword)
	mov [fs:BOOT_INFO + VIDEO_MEMORY], eax		; BootInfo�� ���

	mov ax, 0x4f02								; �׷��� ���� ��ȯ
	mov bx, (0100000b << 9) | VBE_MODE			; ȭ�� ���� Ƚ��=�⺻��, ���� ������ ���� ��, ȭ�� ����
	int 0x10									; BIOS ȣ��
	cmp ax, 0x004f								; �����ϸ� ax�� ���� 0x004f�̴�
	jne vbe_error								; �ƴ϶�� vbe_error�� ����

	; 16-bit �ڵ� �ۼ�}}

	mov al, 0xff								; PIC ���ͷ�Ʈ ����
	out 0x21, al								; ������ PIC
	nop											; ��� �ϸ� �� �� ���� �����Ƿ� ��� ����.
	out 0xa1, al								; �����̺� PIC

	cli											; CPU ���ͷ�Ʈ ����

	call waitkbdout								; Ű���� ��Ʈ�귯�� �غ�� ������ ��ٸ�
	mov al, 0xd1								; Ű���� ��Ʈ�귯 ������ ��Ʈ ��ȣ ���� (0xdl = ��Ÿ ��Ʈ)
	out 0x64, al								; out!

	call waitkbdout								; Ű���� ��Ʈ�귯�� �غ�� ������ ��ٸ�
	mov al, 0xdf								; A20 ����Ʈ Ȱ��ȭ ��ɾ�
	out 0x60, al								; out!

	call waitkbdout

	lgdt [gdtr]									; GDT �ε�

	mov eax, cr0								; cr0���������� ���� �����Ѵ�.
	or eax, 0x00000001							; Protected Mode
	and eax, 0x7fffffff							; ����¡�� ������� ����
	mov cr0, eax								; mov!

	jmp dword SysAsmCodeSelector:PM_Start			; 32��Ʈ Ŀ�η� �����Ѵ�

[bits 32]
PM_Start:
	mov ax, SysDataSelector						; ���׸�Ʈ �ʱ�ȭ
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	mov esp, 0x7c00								; ���� �ʱ�ȭ

	; {{32-bit �ڵ� �ۼ�
	; 32-bit �ڵ� �ۼ�}}

	mov esi, 0x7c00								; �÷��� ��ũ�� ��ġ (��Ʈ����+���� ������)
	mov edi, FLOPPY_CPY_ADD						; ������ ��
	mov ecx, READ_CYLINDERS * 512 * 18 * 2		; ������ �� ���
	call memcpy									; copy!

	mov esi, FLOPPY_CPY_ADD + 0x4200 + C_Kernel	; C��� Ŀ�� ����
	mov edi, KERNEL_CPY_ADD						; ������ ��
	mov ecx, KERNEL_SIZE						; ������ ��
	call memcpy									; copy!

	mov esi, BOOT_INFO
	mov edi, BOOT_INFO_CPY_ADD
	mov ecx, BOOT_INFO_SIZE
	call memcpy

	mov esp, KERNEL_STACK						; ������ KERNEL_STACK���� ����

	jmp SysCodeSelector:KERNEL_CPY_ADD			; Ŀ�η� ����

;-----------;
; Functions ;
;-----------;
memcpy:			; �޸� ���� �Լ�. ds:esi=������ ������ �ּ�, es:edi=����� �ּ�, ecx=ũ��(byte)
	pushad										; ��� ���� �������� �� �Ϻ� �ε��� �������� push

	cld											; ���� �÷���(DF) = 0 -> ���ڿ� ��ɾ� ����� edi, esi ����
	rep movsb									; ����
												; rep = ecx�� 0�� ������ ��ɾ �ݺ�(�ѹ� �ݺ��� ������ ecx����)
												; movsb = mov�� ����Ʈ ���ڿ� ��ɾ�, mov byte [es:edi], [ds:esi]
												; ����Ʈ ���ڿ� ��ɾ��̹Ƿ� ������ edi, esi 1����

	popad										; ��� ���� �������� �� �Ϻ� �ε��� �������� pop
	ret											; return;

waitkbdout:		; Ű���� ��Ʈ�귯 ��� �Լ�
	push eax									; ����ϴ� ��������(eax) push

wait_loop:
	in al, 0x64									; Ű���� ��Ʈ�귯 ���� in
	and al, 0x2									; ���� �غ� �� �������� 2��° bit�� 0�� ��(���� ZF=1)
	in al, 0x60									; Ű���� ��Ʈ�귯�� ���۸� ���
	jnz wait_loop								; ZF=0(Ű���� ��Ʈ�귯�� �غ� �� �Ÿ� ������ ��

	pop eax										; ����ϴ� ��������(eax) pop
	ret

;-----------;
; GDT Table ;
;-----------;
gdtr:
	dw gdt_end - gdt - 1							; GDT�� ũ��
	dd gdt + (KERNEL_SEGMENT * 0x10)			; GDT�� ���� �ּ�

gdt:											; GDT�� ����

NullSelector:									; ����� NULL ��ũ����
	dw 0										; ���� 0
	dw 0
	db 0
	db 0
	db 0
	db 0

SysCodeSelector equ (1 * 8)					; �ڵ� ���׸�Ʈ ��ũ����
	dw 0xffff									; Limit 0~15 bit = 0xffff
	dw 0										; Base 0~15 bit = 0
	db 0										; Base 16~23 bit = 0
	db 10011010b								; P=1, DPL=0, Code, non-conforming, readable
	db 11001111b								; G=1, D=1, Limit 16~19 bit = 0xf (Limit = 0xfffff)
	db 0										; Base 24~32 bit = 0 (Base = 0)

SysDataSelector equ (2 * 8)					; ������ ���׸�Ʈ ��ũ����
	dw 0xffff									; Limit 0~15bit = 0xffff
	dw 0										; Base 0~15 bit = 0
	db 0										; Base 16~23 bit = 0
	db 10010010b								; P=1, DPL=0, Data, expand-up, writable
	db 11001111b								; G=1, D=1, Limit 16~19 bit = 0xf (Limit = 0xfffff)
	db 0										; Base 24~32 bit = 0 (Base = 0)

SysAsmCodeSelector equ (3 * 8)				; �ӽ� �ڵ� ���׸�Ʈ ��ũ����
	dw 0xffff									; Limit 0~15 bit = 0xffff
	dw (KERNEL_SEGMENT * 0x10) & 0xffff			; Base 0~15 bit
	db (KERNEL_SEGMENT * 0x10) >> 16			; Base 16~23 bit
	db 10011010b								; P=1, DPL=0, Code, non-conforming, readable
	db 11001111b								; G=1, D=1, Limit 16~19 bit = 0xf (Limit = 0xfffff)
	db ((KERNEL_SEGMENT * 0x10) >> 24) & 0xff	; Base 24~32 bit = 0 (Base = KERNEL_SEGMENT * 0x10)

gdt_end:										; GDT�� �� + 1

C_Kernel:										; C���� ���� Ŀ���� ����
