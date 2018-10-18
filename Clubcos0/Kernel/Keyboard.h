#ifndef Keyboard_h_
#define Keyboard_h_

#include "Clubcos0.h"

#define KEYBOARD_CONTROL_PORT 0x64
#define KEYBOARD_STATE_PORT 0x64
#define KEYBOARD_BUFFER_PORT 0x60

#define KEYBOARD_CONTROL_ACTIVATE 0xae

#define KEYBOARD_CONTROL_READ_CMD 0x20
#define KEYBOARD_CONTROL_WRITE_CMD 0x60

#define KEYBOARD_ACTIVATE 0xf4
#define KEYBOARD_ACK 0xfa
#define KEYBOARD_LED 0xed

#include "KeyTable.h"

typedef struct tagKeyStruct
{
	BYTE NormalKey;
	BYTE CombinedKey;
	BOOL bPushedKey;
} KeyStruct;

typedef struct tagKeyboardStruct
{
	BOOL bNumLock;
	BOOL bCapsLock;
	BOOL bScrollLock;
	BOOL bShift;
} KeyboardStruct;

extern KeyStruct g_KeyTable[];
extern KeyboardStruct g_Keyboard;

WORD GetKeyCodeFromScanCode(BYTE ScanCode, BOOL bExtern, BOOL bPause);
BYTE GetClubcos0AsciiFromKeyCode(WORD KeyCode);
BOOL IsAsciiCode(BYTE Clubcos0Ascii);
BOOL IsDownKey(WORD KeyCode);

BOOL InitKeyboard();
BOOL UpdateKeyboardLED();

void WaitKeyboardInputBuffer();
void WaitKeyboardOutputBuffer();
BOOL WaitKeyboardACK();

BOOL IsKeyboardOutputBufferFull();
BOOL IsKeyboardInputBufferFull();

void PutScanCodeToInterruptQueue(BYTE ScanCode);

#endif // Keyboard_h_
