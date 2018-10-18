#ifndef IDT_h_
#define IDT_h_

#include "Clubcos0.h"

#define IDT_ADDRESS 0x620000

typedef void (*InterruptHandler)();

typedef BYTE IdtType;

typedef struct tagIdt
{
	WORD HandlerAddress_0_15;
	
	WORD HandlerSegment;

	BYTE ReservedZero1;

	union
	{
		struct 
		{
			BYTE ReservedZero2:1, ReservedOne1:1, ReservedOne2:1;

			BYTE D:1;

			BYTE ReservedZero3:1;

			BYTE DPL:2, P:1;
		};
		IdtType type;
	};

	WORD HandlerAddress_16_31;
} Idt;

void InitIdt(Idt *pIdt, InterruptHandler HandlerAddress, WORD HandlerSegment, BYTE DPL);

#endif // IDT_h_
