#ifndef Mouse_h_
#define Mouse_h_

#include "Clubcos0.h"

#define MOUSE_CONTROL_ACTIVATE 0xa8
#define MOUSE_CONTROL_DATA_TO_MOUSE 0xd4

#define KEYBOARD_CMD_MOUSE_INTERRUPT_ENABLE_BIT 0x02

#define MOUSE_ACTIVATE 0xf4

typedef union tagMouseFlag
{
	BYTE flag;
	struct 
	{
		BYTE LeftBtn:1, RightBtn:1, CenterBtn:1, Reserved1:1, SignX:1, SignY:1, OverflowX:1, OverflowY:1;
	};
} MouseFlag;

typedef union tagMouseData
{
	DWORD data;
	struct
	{
		MouseFlag flag;
		SBYTE dx, dy;
	};
} MouseData;

typedef struct tagMouseStruct
{
	int x, y;
	MouseFlag flag;
} MouseStruct;

extern MouseStruct g_Mouse;

BOOL InitMouse();
void EnableMouseInterrupt();

void PutMouseCodeToInterruptQueue(BYTE Code);

void ApplyMouseData(MouseData data);
void DrawMouse(int x, int y);

#endif // Mouse_h_
