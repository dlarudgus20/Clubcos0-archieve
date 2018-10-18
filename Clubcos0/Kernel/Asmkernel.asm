%include "../Bootsector/init.inc"

; VBE
VBE_INFO_SEGMENT equ 0x0200						; VBE의 정보가 저장되는 세그먼트의 위치
VBE_MODE equ 0x117								; VBE 화면 모드

; BootInfo
BOOT_INFO equ 0x7000							; BootInfo의 위치
READ_CYLS equ 0									; 읽어들인 실린더 수
SCREEN_X equ 1									; 화면의 X좌표 크기
SCREEN_Y equ 3									; 화면의 Y좌표 크기
VIDEO_MEMORY equ 5								; 비디오 메모리의 포인터
BOOT_INFO_SIZE equ 9							; BootInfo의 크기

; 복사
FLOPPY_CPY_ADD equ 0x100000						; 플로피 디스크의 데이터가 복사되는 주소
BOOT_INFO_CPY_ADD equ 0x268000					; BootInfo가 복사되는 주소
KERNEL_CPY_ADD equ 0x500000						; 커널이 복사되는 주소
KERNEL_STACK equ 0x500000						; 커널 스택의 주소
KERNEL_SIZE equ (1024*540)						; 커널의 최대 크기 = 540KB

[org 0]
[bits 16]

Kernel_start:
	mov ax, cs									; cs와 ds를 같게 한다
	mov ds, ax

	mov bx, 0
	mov fs, bx									; fs = 0

	; {{16-bit 코드 작성

	mov byte [fs:BOOT_INFO + READ_CYLS], READ_CYLINDERS ; 몇 실린더를 읽었는지 BootInfo에 기록

	mov ax, VBE_INFO_SEGMENT
	mov es, ax
	mov di, 0									; es:di에 VBE정보가 들어간다
	mov ax, 0x4f00								; ax=0x4f00 -> VBE정보 및 존재 유무 확인
	int 0x10									; BIOS 호출
	cmp ax, 0x004f								; VBE가 있다면 ax의 값은 0x004f이다
	jne vbe_error								; ax가 0x004f가 아니라면 vbe_error로 간다

	mov ax, [es:di + 4]							; es:di + 4에는 버전 정보가 있다.
	cmp ax, 0x0200								; 버전이 0x0200인지(2.0인지) 이상인지 확인한다
	jb vbe_error								; 0x0200 미만이라면 vbe_error로 간다

	jmp vbe_success								; vbe_success로 간다

vbe_error:
	mov si, vbe_error_msg						; 에러 메세지의 주소
putloop:
	mov al, [si]								; 메세지의 한 글자 읽음
	inc si										; si++
	cmp al, 0									; al과 0을 비교
	je stop										; 0이면 stop으로 jmp
	mov ah, 0x0e								; ah=0x0e -> 화면에 한 글자 출력
	mov bx, 15									; 색 코드
	int 0x10									; BIOS 호출(al엔 글자 코드)
	jmp putloop									; 루프를 돔
stop:
	hlt											; CPU를 일시정지
	jmp stop									; 무한루프

vbe_error_msg db "Clubcos0 requires VBE 2.0, but this computer doesn't support VBE 2.0. System stop.", 0

vbe_success:
	mov ax, VBE_INFO_SEGMENT
	mov es, ax
	mov di, 0									; es:di에 화면 모드 정보가 들어간다
	mov ax, 0x4f01								; ax=0x4f01 -> 화면 모드 지원 여부 및 정보 확인
	mov cx, VBE_MODE								; cx -> 화면 모드
	int 0x10									; BIOS 호출
	cmp ax, 0x004f								; 성공하면 ax의 값은 0x004f이다
	jne vbe_error								; 아니라면 vbe_error로 간다

	mov ax, [es:di + 0x12]						; 화면 모드+0x12 -> 화면 X방향 크기(2byte = word)
	mov [fs:BOOT_INFO + SCREEN_X], ax			; BootInfo에 기록
	mov ax, [es:di + 0x14]						; 화면 모드+0x14 -> 화면 Y방향 크기(2byte = word)
	mov [fs:BOOT_INFO + SCREEN_Y], ax			; BootInfo에 기록
	mov eax, [es:di + 0x28]						; 화면 모드 정보+0x28 -> 비디오 메모리의 위치(pointer = 4byte = dword)
	mov [fs:BOOT_INFO + VIDEO_MEMORY], eax		; BootInfo에 기록

	mov ax, 0x4f02								; 그래픽 모드로 전환
	mov bx, (0100000b << 9) | VBE_MODE			; 화면 갱신 횟수=기본값, 선형 프레임 버퍼 모델, 화면 지움
	int 0x10									; BIOS 호출
	cmp ax, 0x004f								; 성공하면 ax의 값은 0x004f이다
	jne vbe_error								; 아니라면 vbe_error로 간다

	; 16-bit 코드 작성}}

	mov al, 0xff								; PIC 인터럽트 금지
	out 0x21, al								; 마스터 PIC
	nop											; 계속 하면 안 될 때도 있으므로 잠시 쉰다.
	out 0xa1, al								; 슬레이브 PIC

	cli											; CPU 인터럽트 금지

	call waitkbdout								; 키보드 컨트룰러가 준비될 때까지 기다림
	mov al, 0xd1								; 키보드 컨트룰러 내부의 포트 번호 지정 (0xdl = 기타 포트)
	out 0x64, al								; out!

	call waitkbdout								; 키보드 컨트룰러가 준비될 때까지 기다림
	mov al, 0xdf								; A20 게이트 활성화 명령어
	out 0x60, al								; out!

	call waitkbdout

	lgdt [gdtr]									; GDT 로드

	mov eax, cr0								; cr0레지스터의 값을 변경한다.
	or eax, 0x00000001							; Protected Mode
	and eax, 0x7fffffff							; 페이징을 사용하지 않음
	mov cr0, eax								; mov!

	jmp dword SysAsmCodeSelector:PM_Start			; 32비트 커널로 점프한다

[bits 32]
PM_Start:
	mov ax, SysDataSelector						; 세그먼트 초기화
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	mov esp, 0x7c00								; 스택 초기화

	; {{32-bit 코드 작성
	; 32-bit 코드 작성}}

	mov esi, 0x7c00								; 플로피 디스크의 위치 (부트섹터+읽은 데이터)
	mov edi, FLOPPY_CPY_ADD						; 복사할 곳
	mov ecx, READ_CYLINDERS * 512 * 18 * 2		; 복사할 양 계산
	call memcpy									; copy!

	mov esi, FLOPPY_CPY_ADD + 0x4200 + C_Kernel	; C언어 커널 복사
	mov edi, KERNEL_CPY_ADD						; 복사할 곳
	mov ecx, KERNEL_SIZE						; 복사할 양
	call memcpy									; copy!

	mov esi, BOOT_INFO
	mov edi, BOOT_INFO_CPY_ADD
	mov ecx, BOOT_INFO_SIZE
	call memcpy

	mov esp, KERNEL_STACK						; 스택을 KERNEL_STACK으로 설정

	jmp SysCodeSelector:KERNEL_CPY_ADD			; 커널로 점프

;-----------;
; Functions ;
;-----------;
memcpy:			; 메모리 복사 함수. ds:esi=복사할 데이터 주소, es:edi=복사될 주소, ecx=크기(byte)
	pushad										; 모든 범용 레지스터 및 일부 인덱스 레지스터 push

	cld											; 방향 플래그(DF) = 0 -> 문자열 명령어 실행시 edi, esi 증가
	rep movsb									; 복사
												; rep = ecx가 0일 때까지 명령어를 반복(한번 반복할 때마다 ecx감소)
												; movsb = mov의 바이트 문자열 명령어, mov byte [es:edi], [ds:esi]
												; 바이트 문자열 명령어이므로 실행후 edi, esi 1증가

	popad										; 모든 범용 레지스터 및 일부 인덱스 레지스터 pop
	ret											; return;

waitkbdout:		; 키보드 컨트룰러 대기 함수
	push eax									; 사용하는 레지스터(eax) push

wait_loop:
	in al, 0x64									; 키보드 컨트룰러 상태 in
	and al, 0x2									; 만일 준비가 안 돼있으면 2번째 bit가 0이 됨(따라서 ZF=1)
	in al, 0x60									; 키보드 컨트룰러의 버퍼를 비움
	jnz wait_loop								; ZF=0(키보드 컨트룰러가 준비가 안 돼면 루프를 돔

	pop eax										; 사용하는 레지스터(eax) pop
	ret

;-----------;
; GDT Table ;
;-----------;
gdtr:
	dw gdt_end - gdt - 1							; GDT의 크기
	dd gdt + (KERNEL_SEGMENT * 0x10)			; GDT의 시작 주소

gdt:											; GDT의 시작

NullSelector:									; 예약된 NULL 디스크럽터
	dw 0										; 전부 0
	dw 0
	db 0
	db 0
	db 0
	db 0

SysCodeSelector equ (1 * 8)					; 코드 세그먼트 디스크럽터
	dw 0xffff									; Limit 0~15 bit = 0xffff
	dw 0										; Base 0~15 bit = 0
	db 0										; Base 16~23 bit = 0
	db 10011010b								; P=1, DPL=0, Code, non-conforming, readable
	db 11001111b								; G=1, D=1, Limit 16~19 bit = 0xf (Limit = 0xfffff)
	db 0										; Base 24~32 bit = 0 (Base = 0)

SysDataSelector equ (2 * 8)					; 데이터 세그먼트 디스크럽터
	dw 0xffff									; Limit 0~15bit = 0xffff
	dw 0										; Base 0~15 bit = 0
	db 0										; Base 16~23 bit = 0
	db 10010010b								; P=1, DPL=0, Data, expand-up, writable
	db 11001111b								; G=1, D=1, Limit 16~19 bit = 0xf (Limit = 0xfffff)
	db 0										; Base 24~32 bit = 0 (Base = 0)

SysAsmCodeSelector equ (3 * 8)				; 임시 코드 세그먼트 디스크럽터
	dw 0xffff									; Limit 0~15 bit = 0xffff
	dw (KERNEL_SEGMENT * 0x10) & 0xffff			; Base 0~15 bit
	db (KERNEL_SEGMENT * 0x10) >> 16			; Base 16~23 bit
	db 10011010b								; P=1, DPL=0, Code, non-conforming, readable
	db 11001111b								; G=1, D=1, Limit 16~19 bit = 0xf (Limit = 0xfffff)
	db ((KERNEL_SEGMENT * 0x10) >> 24) & 0xff	; Base 24~32 bit = 0 (Base = KERNEL_SEGMENT * 0x10)

gdt_end:										; GDT의 끝 + 1

C_Kernel:										; C언어로 만든 커널의 시작
