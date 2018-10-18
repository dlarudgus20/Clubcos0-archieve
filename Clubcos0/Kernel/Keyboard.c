#include "Clubcos0.h"
#include "Keyboard.h"
#include "AssemblyFunc.h"
#include "Functions.h"
#include "Queue.h"
#include "KernelData.h"
#include "Interrupt.h"
#include "Pic.h"
#include "Graphic.h"

KeyboardStruct g_Keyboard = { FALSE, FALSE, FALSE, FALSE };

WORD GetKeyCodeFromScanCode(BYTE ScanCode, BOOL bExtern, BOOL bPause)
{
	BOOL bPushKey = TRUE;
	WORD ret;

	if((ScanCode & 0x80) != 0)
	{
		bPushKey = FALSE;
		ScanCode &= 0x7f;
	}

	if(bPause)
	{
		ret = KEY_CODE_PAUSE;
	}
	else if(bExtern)
	{
		ret = ScanCode - 0x1C + 0x59;
	}
	else
	{
		ret = ScanCode;
	}

	return bPushKey ? ret : (ret | KEY_UNPUSH);
}

BYTE GetClubcos0AsciiFromKeyCode(WORD KeyCode)
{
	KeyStruct key;

	KeyCode &= 0xff;

	key = g_KeyTable[KeyCode];
	if(KeyCode >= KEY_NUM_MIN && KeyCode <= KEY_NUM_MAX)
	{
		if(g_Keyboard.bNumLock)
		{
			return key.CombinedKey;
		}
		else
		{
			return key.NormalKey;
		}
	}
	else if(isalpha(key.NormalKey))
	{
		if(g_Keyboard.bCapsLock)
		{
			if(g_Keyboard.bShift)
			{
				return key.NormalKey;
			}
			else
			{
				return key.CombinedKey;
			}
		}
		else if(g_Keyboard.bShift)
		{
			return key.CombinedKey;
		}
		else
		{
			return key.NormalKey;
		}
	}
	else
	{
		if(g_Keyboard.bShift)
		{
			return key.CombinedKey;
		}
		else
		{
			return key.NormalKey;
		}
	}
}

BOOL IsAsciiCode(BYTE Clubcos0Ascii)
{
	return ((Clubcos0Ascii & 0x80) != 0) ? FALSE : TRUE;
}

BOOL IsDownKey(WORD KeyCode)
{
	return ((KeyCode & KEY_UNPUSH) == 0);
}

BOOL InitKeyboard()
{
	g_Keyboard.bNumLock = TRUE;

	OutByte(KEYBOARD_CONTROL_PORT, KEYBOARD_CONTROL_ACTIVATE);

	WaitKeyboardInputBuffer();

	OutByte(KEYBOARD_BUFFER_PORT, KEYBOARD_ACTIVATE);

	if(WaitKeyboardACK())
	{
		UpdateKeyboardLED();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL UpdateKeyboardLED()
{
	DWORD eflag = GetEFlag();
	BOOL bSuccess;

	AsmCli();

	WaitKeyboardInputBuffer();

	OutByte(KEYBOARD_BUFFER_PORT, KEYBOARD_LED);

	WaitKeyboardInputBuffer();
	if(!WaitKeyboardACK())
	{
		return FALSE;
	}

	OutByte(KEYBOARD_BUFFER_PORT, (g_Keyboard.bCapsLock << 2) | (g_Keyboard.bNumLock << 1) | g_Keyboard.bScrollLock);

	WaitKeyboardInputBuffer();

	bSuccess = WaitKeyboardACK();

	SetEFlag(eflag);
	return bSuccess;
}

void WaitKeyboardInputBuffer()
{
	int i;

	for(i = 0; i < 0xffff; i++)
	{
		if(!IsKeyboardInputBufferFull())
		{
			break;
		}
	}
}

void WaitKeyboardOutputBuffer()
{
	int i;

	for(i = 0; i < 0xffff; i++)
	{
		if(!IsKeyboardOutputBufferFull())
		{
			break;
		}
	}
}

BOOL WaitKeyboardACK()
{
	BYTE ScanCode;
	int i, j;

	for(i = 0; i < 100; i++)
	{
		for(j = 0; j < 0xffff; j++)
		{
			if(IsKeyboardOutputBufferFull())
			{
				break;
			}
		}

		ScanCode = InByte(KEYBOARD_BUFFER_PORT);
		if(ScanCode == KEYBOARD_ACK)
		{
			return TRUE;
		}
		else
		{
			PutScanCodeToInterruptQueue(ScanCode);
		}
	}

	return FALSE;
}

BOOL IsKeyboardOutputBufferFull()
{
	if((InByte(KEYBOARD_STATE_PORT) & 0x1) != 0)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL IsKeyboardInputBufferFull()
{
	if((InByte(KEYBOARD_STATE_PORT) & 0x2) != 0)
	{
		return TRUE;
	}
	return FALSE;
}

void PutScanCodeToInterruptQueue(BYTE ScanCode)
{
	static BOOL bExternCode = FALSE, bPause = FALSE;
	static DWORD SkipForPause = 0;

	if(bPause)
	{
		SkipForPause--;
		if(SkipForPause != 0)
		{
			return;
		}
		else
		{
			bPause = FALSE;
			PutQueue32(&g_kData.InterruptQueue, KEY_CODE_PAUSE | INTERRUPT_QUEUE_FLAG_KEYBOARD);
			return;
		}
	}
	else if(bExternCode)
	{
		bExternCode = FALSE;
		PutQueue32(&g_kData.InterruptQueue, GetKeyCodeFromScanCode(ScanCode, TRUE, FALSE) | INTERRUPT_QUEUE_FLAG_KEYBOARD);
	}
	else
	{
		if(ScanCode == 0xE1)
		{
			bPause = TRUE;
			SkipForPause = 2;
			return;
		}
		else if(ScanCode == 0xE0)
		{
			bExternCode = TRUE;
			return;
		}
		else
		{
			PutQueue32(&g_kData.InterruptQueue, GetKeyCodeFromScanCode(ScanCode, FALSE, FALSE) | INTERRUPT_QUEUE_FLAG_KEYBOARD);
		}
	}
}

void OnKeyboardInterrupt(DWORD QueueData)
{
	BYTE CAsciiCode;

	CAsciiCode = GetClubcos0AsciiFromKeyCode(QueueData);

	g_KeyTable[QueueData & 0xff].bPushedKey = IsDownKey(QueueData);
	if (!IsAsciiCode(CAsciiCode))
	{
		switch (CAsciiCode)
		{
		case KEY_LSHIFT:
		case KEY_RSHIFT:
			g_Keyboard.bShift =
				(g_KeyTable[SCAN_CODE_LSHIFT].bPushedKey || g_KeyTable[SCAN_CODE_RSHIFT].bPushedKey);
			break;
		case KEY_NUMLOCK:
			if (!IsDownKey(QueueData))
			{
				g_Keyboard.bNumLock = !g_Keyboard.bNumLock;
				UpdateKeyboardLED();
			}
			break;
		case KEY_CAPSLOCK:
			if (!IsDownKey(QueueData))
			{
				g_Keyboard.bCapsLock = !g_Keyboard.bCapsLock;
				UpdateKeyboardLED();
			}
			break;
		case KEY_SCROLLLOCK:
			if(!IsDownKey(QueueData))
			{
				g_Keyboard.bScrollLock = !g_Keyboard.bScrollLock;
				UpdateKeyboardLED();
			}
			break;
		}
	}

	DrawBoxFill(0, 30, GetBootInfo()->ScreenX, 46, BACK_COLOR);
	if(IsAsciiCode(CAsciiCode))
	{
		DrawStringFormat(0, 30, BLACK, "Input Ascii Code [%c]", CAsciiCode);
	}
	else
	{
		DrawStringFormat(0, 30, BLACK, "Input Non Ascii Code [0x%x]", CAsciiCode);
	}
}

void _KeyboardHandler(DWORD *esp)
{
	BYTE ScanCode;

	SendEOI(PIC_IRQ_KEYBOARD);
	ScanCode = InByte(KEYBOARD_BUFFER_PORT);

	PutScanCodeToInterruptQueue(ScanCode);
}
