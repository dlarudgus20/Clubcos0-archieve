%include "init.inc"

[org 0x7c00]
[bits 16]
	jmp start

; fat12 파일 시스템의 헤더
db 0						; fat12의 헤더가 이 다음부터 시작하기 때문에, 1byte를 넣음
db "CLUBCOS0"				; 부트섹터의 이름을 8글자로 적음
dw 512						; 1섹터의 크기(바이트 단위)
db 1						; 클러스터의 크기(섹터 단위, fat12에선 1섹터
dw 1						; 예약된 섹터의 수
db 2						; 디스크의 FAT의 수(fat12에선 2)
dw 224						; 루트 디렉터리 엔트리의 수(보통은 224엔트리)
dw 2880						; 디스크의 총 섹터 수
db 0xf0						; 미디어 타입(0xf0으로 해야 함)
dw 9						; FAT 하나당 섹터 수(fat12에선 9)
dw 18						; 1트랙당 섹터 수(플로피 디스크에선 18)
dw 2						; 헤드의 수
dd 0						; 파티션을 쓰지 않으므로 0
dd 2880						; 섹터 수를 다시 씀
db 0, 0, 0x29				; 잘 모르겠습니다ㅡ.ㅡ
dd 0xffffffff				; 책에선 '아마 볼륨 시리얼 번호'라네요
db "CLUBCOS0   "			; 디스크의 이름(11바이트)
db "FAT12   "				; 파일 시스템의 이름(8바이트)
times 18 db 0				; 18바이트 비움

start:
	mov ax, cs
	mov ds, ax				; ds를 cs와 같게 한다

	mov ax, 0				; 스택을 설정한다
	mov ss, ax				; 스택의 세그먼트 : 0
	mov sp, 0x7c00			; 스택의 오프셋 : 0x7c00

	mov si, boot_msg		; 부팅 메세지 출력문
bootmsg_putloop:
	mov al, [si]			; 메세지의 한 글자 읽음
	inc si					; si++
	cmp al, 0				; al과 0을 비교
	je bootmsg_end			; 0이면 bootmsg_end으로 jmp
	mov ah, 0x0e			; ah=0x0e -> 화면에 한 글자 출력
	mov bx, 15				; 색 코드
	int 0x10				; BIOS 호출(al엔 글자 코드)
	jmp bootmsg_putloop		; 루프를 돔

boot_msg:
	db 0x0a, 0x0a			; 개행 ASCII 코드
	db "This is Clubcos0 boot loader, booting...."
	db 0x0a, 0x0d
	db 0					; NULL
bootmsg_end:

	mov ax, FLOPPY_SEGMENT
	mov es, ax				; es:bx -> 읽기 시작할 메모리의 주소(bx는 0)
	mov ch, 0				; 실린더 0
	mov dh, 0				; 헤드 0
	mov cl, 2				; 섹터 2
readloop:
	mov si, 0				; 읽기 실패 횟수를 세는 레지스터
retry:
	mov ah, 0x02			; ah=2 -> 읽기
	mov al, 1				; 1섹터씩 읽음
	mov bx, 0
	mov dl, 0				; 첫번째 플로피 디스크
	int 0x13				; BIOS 호출
	jnc next				; 성공이면 next로
	inc si					; 읽기 실패 횟수 증가
	cmp si, 5				; si와 5를 비교
	jae error				; 5 이상이면 에러 처리
	mov ah, 0				; ah=0 -> 드라이브 리셋
	mov dh, 0				; 첫번째 플로피 디스크
	int 0x13				; BIOS 호출
	jmp retry				; 다시 시도
next:
	mov ax, es				; es:bx에 0x200을 더함
	add ax, 0x20			; es는 세그먼트이기 때문에 0x20을 더함
	mov es, ax				; es는 세그먼트이기 때문에 ax를 경유해서 계산
	inc cl					; 섹터 번호를 1증가
	cmp cl, 18				; cl과 18을 비교
	jbe readloop			; 18 이하라면 readloop로 jmp
	mov cl, 1				; 18 초과라면 섹터 번호를 1로
	inc dh					; 그리고 헤드 번호를 1증가
	cmp dh, 2				; dh와 2를 비교
	jb readloop				; 2 미만이라면 readloop로 jmp
	mov dh, 0				; 2 이상이라면 dh를 0으로
	inc ch					; 실린더 번호를 증가
	cmp ch, READ_CYLINDERS	; ch와 READ_CYLINDERS를 비교
	jb readloop				; ch < READ_CYLINDERS(다 못읽었으면)
							; readloop로 감

	jmp KERNEL_SEGMENT:0	; OS를 실행!

error:						; 에러 처리 구문
	mov si, msg				; 에러 메세지의 주소
putloop:
	mov al, [si]			; 메세지의 한 글자 읽음
	inc si					; si++
	cmp al, 0				; al과 0을 비교
	je stop					; 0이면 stop으로 jmp
	mov ah, 0x0e			; ah=0x0e -> 화면에 한 글자 출력
	mov bx, 15				; 색 코드
	int 0x10				; BIOS 호출(al엔 글자 코드)
	jmp putloop				; 루프를 돔
stop:
	hlt						; CPU를 일시정지
	jmp stop				; 무한루프

msg:
	db 0x0a, 0x0a			; 개행 ASCII 코드
	db "Clubcos0 load error, system halt"
	db 0x0a
	db 0					; NULL

times 510-($-$$) db 0
dw 0xAA55