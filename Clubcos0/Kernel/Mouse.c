#include "Clubcos0.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "AssemblyFunc.h"
#include "Queue.h"
#include "KernelData.h"
#include "Graphic.h"
#include "Interrupt.h"
#include "Functions.h"
#include "PIC.h"

MouseStruct g_Mouse = { 0 };

BOOL InitMouse()
{
	BOOL bSuccess;
	DWORD eflag;

	eflag = GetEFlag();
	AsmCli();

	g_Mouse.x = GetBootInfo()->ScreenX / 2;
	g_Mouse.y = GetBootInfo()->ScreenY / 2;

	OutByte(KEYBOARD_CONTROL_PORT, MOUSE_CONTROL_ACTIVATE);

	OutByte(KEYBOARD_CONTROL_PORT, MOUSE_CONTROL_DATA_TO_MOUSE);

	WaitKeyboardInputBuffer();

	OutByte(KEYBOARD_BUFFER_PORT, MOUSE_ACTIVATE);

	bSuccess = WaitKeyboardACK();

	SetEFlag(eflag);

	if(bSuccess)
	{
		EnableMouseInterrupt();

		DrawMouse(g_Mouse.x, g_Mouse.y);
	}
	return bSuccess;
}

void EnableMouseInterrupt()
{
	BYTE KeyboardCmd;

	OutByte(KEYBOARD_CONTROL_PORT, KEYBOARD_CONTROL_READ_CMD);

	WaitKeyboardOutputBuffer();

	KeyboardCmd = InByte(KEYBOARD_BUFFER_PORT);

	KeyboardCmd |= KEYBOARD_CMD_MOUSE_INTERRUPT_ENABLE_BIT;

	OutByte(KEYBOARD_CONTROL_PORT, KEYBOARD_CONTROL_WRITE_CMD);

	WaitKeyboardInputBuffer();

	OutByte(KEYBOARD_BUFFER_PORT, KeyboardCmd);
}

void PutMouseCodeToInterruptQueue(BYTE Code)
{
	static MouseData data;
	static DWORD phase = 4;

	switch (phase)
	{
	case 4:
		if (Code == 0xfa)
			phase--;
		break;
	case 3:
		if ((Code & 0xc8) == 0x08)
		{
			data.flag.flag = Code;
			phase--;
		}
		break;
	case 2:
		data.dx = (SBYTE)Code;
		phase--;
		break;
	case 1:
		data.dy = (SBYTE)Code;

		PutQueue32(&g_kData.InterruptQueue, data.data | INTERRUPT_QUEUE_FLAG_MOUSE);

		phase = 3;
		break;
	}
}

void ApplyMouseData(MouseData data)
{
	DrawBoxFill(g_Mouse.x, g_Mouse.y, g_Mouse.x + 13, g_Mouse.y + 19, BACK_COLOR);

	g_Mouse.x = range(0, g_Mouse.x + data.dx, GetBootInfo()->ScreenX - 1);
	g_Mouse.y = range(0, g_Mouse.y - data.dy, GetBootInfo()->ScreenY - 1);
	g_Mouse.flag = data.flag;

	DrawMouse(g_Mouse.x, g_Mouse.y);
}

void DrawMouse(int x, int y)
{
	static char MouseBlock[] = { /* 13x19 Block */
		"*............"
		"**..........."
		"*-*.........."
		"*o-*........."
		"*oo-*........"
		"*ooo-*......."
		"*oooo-*......"
		"*ooooo-*....."
		"*oooooo-*...."
		"*ooooooo-*..."
		"*oooooooo-*.."
		"*ooooooooo-*."
		"*oooooo******"
		"*ooo*-o*....."
		"*oo*.*oo*...."
		"*-*..*-o*...."
		"**....*oo*..."
		"*.....*--*..."
		".......**...."
	};


	DrawBlock(MouseBlock, x, y, 13, 19, BACK_COLOR);
}

void OnMouseInterrupt(DWORD QueueData)
{
	MouseData data;

	data.data = QueueData;

	ApplyMouseData(data);
}

void _MouseHandler(DWORD *esp)
{
	BYTE Code;

	SendEOI(PIC_IRQ_MOUSE);
	Code = InByte(KEYBOARD_BUFFER_PORT);

	PutMouseCodeToInterruptQueue(Code);
}
