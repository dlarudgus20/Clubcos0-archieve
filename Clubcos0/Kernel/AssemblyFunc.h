#ifndef AssemblyFunc_h_
#define AssemblyFunc_h_

#include "Clubcos0.h"
#include "GDT.h"
#include "IDT.h"

// function
void LoadGdt(WORD size, Gdt *address);
void LoadIdt(WORD size, Idt *address);
void LoadTr(DWORD tr);

BYTE InByte(BYTE port);
WORD InWord(BYTE port);
void OutByte(BYTE port, BYTE data);
void OutWord(BYTE port, WORD data);

void memcpy(void *dest, const void *src, unsigned size);
void memset(void *mem, BYTE c, unsigned size);
void memset_2(void *mem, WORD s, unsigned size);
void memset_4(void *mem, DWORD i, unsigned size);
void *memchr_4(void *mem, DWORD i, unsigned size);

void AsmCli();
void AsmSti();
void AsmHlt();
void AsmStiHlt();

DWORD GetEFlag();
void SetEFlag(DWORD eflag);

DWORD GetCr0();
void SetCr0(DWORD cr0);

void FarJmp(DWORD eip, WORD cs);

// EFlag
#define EFLAG_AC 0x00040000
#define EFLAG_IF 0x00000200

#define EFLAG_2 0x2		// eflag���� �� ��Ʈ�� Ȱ��ȭ�Ǿ� �ϴ� �� ����.

// cr0
#define CR0_CACHE_DISABLE 0x60000000

#endif // AssemblyFunc_h_
