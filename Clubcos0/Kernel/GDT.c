#include "Clubcos0.h"
#include "GDT.h"
#include "AssemblyFunc.h"

void InitGdt(Gdt *pGdt, DWORD address, DWORD size, GdtType1 type1)
{
	DWORD Size20bit;

	pGdt->Address_0_15 = (WORD) address;
	pGdt->Address_16_23 = (BYTE)((address >> 16) & 0xff);
	pGdt->Address_24_32 = (BYTE)((address >> 24) & 0xff);

	if(size >= 0xfffff)
	{
		pGdt->G = 1;
		Size20bit = size >> 12;
	}
	else
	{
		pGdt->G = 0;
		Size20bit = size;
	}
	pGdt->Size_0_15 = (WORD) Size20bit;
	pGdt->Size_16_19 = (WORD)((Size20bit >> 16) & 0x0f);

	pGdt->type1 = type1;

	pGdt->AVL = 0;
	pGdt->reserved = 0;
	pGdt->D = 1;
}

void InitNullGdt(Gdt *pGdt)
{
	((int *) pGdt)[0] = 0;
	((int *) pGdt)[1] = 0;
}
