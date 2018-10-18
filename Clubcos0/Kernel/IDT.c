#include "Clubcos0.h"
#include "IDT.h"
#include "AssemblyFunc.h"

void InitIdt(Idt *pIdt, InterruptHandler HandlerAddress, WORD HandlerSegment, BYTE DPL)
{
	pIdt->HandlerAddress_0_15 = (WORD)(DWORD) (HandlerAddress);
	pIdt->HandlerAddress_16_31 = (WORD)((((DWORD) (HandlerAddress)) >> 16) & 0xffff);

	pIdt->HandlerSegment = HandlerSegment;

	pIdt->ReservedZero1 = 0;
	pIdt->ReservedZero2 = 0;
	pIdt->ReservedZero3 = 0;

	pIdt->ReservedOne1 = 1;
	pIdt->ReservedOne2 = 1;

	pIdt->D = 1;
	pIdt->P = 1;

	pIdt->DPL = DPL;
}
