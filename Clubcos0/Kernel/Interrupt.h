#ifndef Interrupt_h_
#define Interrupt_h_

#include "Clubcos0.h"

#define INTERRUPT_QUEUE_FLAG_TIMER		(0x01 << 24)
#define INTERRUPT_QUEUE_FLAG_KEYBOARD	(0x02 << 24)
#define INTERRUPT_QUEUE_FLAG_MOUSE		(0x03 << 24)

typedef struct tagInterruptContext
{
	DWORD gs;
	DWORD fs;
	DWORD es;
	DWORD ds;
	DWORD ebp;
	DWORD edi;
	DWORD esi;
	DWORD edx;
	DWORD ecx;
	DWORD ebx;
	DWORD eax;

	union
	{
		struct
		{
			DWORD eip;
			DWORD cs;
			DWORD eflag;
			DWORD esp2;
			DWORD ss2;
		};
		struct
		{
			DWORD err_code;
			DWORD err_eip;
			DWORD err_cs;
			DWORD err_eflag;
			DWORD err_esp2;
			DWORD err_ss2;
		};
	};
} InterruptContext;

/* 디폴트 핸들러 */
void DefaultHandler0();
void DefaultHandler1();
void DefaultHandler2();
void DefaultHandler3();
void DefaultHandler4();
void DefaultHandler5();
void DefaultHandler6();
void DefaultHandler7();
void DefaultHandler8();
void DefaultHandler9();
void DefaultHandler10();
void DefaultHandler11();
void DefaultHandler12();
void DefaultHandler13();
void DefaultHandler14();
void DefaultHandler15();
void DefaultHandler16();
void DefaultHandler17();
void DefaultHandler18();
void DefaultHandler19();
void DefaultHandler20();

/* 타이머 핸들러 */
void TimerHandler();

/* 키보드 핸들러 */
void KeyboardHandler();
void OnKeyboardInterrupt(DWORD QueueData);

/* 마우스 핸들러 */
void MouseHandler();
void OnMouseInterrupt(DWORD QueueData);

#endif // Interrupt_h_
