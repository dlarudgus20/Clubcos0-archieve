#ifndef KeyTable_h_
#define KeyTable_h_

/* 키 코드 */
#define KEY_NONE			0x00
#define KEY_ENTER			'\n'
#define KEY_TAB				'\t'
#define KEY_ESC				0x1B
#define KEY_BACKSPACE		0x08

#define KEY_LCTRL			0x81
#define KEY_LSHIFT			0x82
#define KEY_RSHIFT			0x83
#define KEY_PRINTSCREEN		0x84
#define KEY_LALT			0x85
#define KEY_CAPSLOCK		0x86
#define KEY_F1				0x87
#define KEY_F2				0x88
#define KEY_F3				0x89
#define KEY_F4				0x8A
#define KEY_F5				0x8B
#define KEY_F6				0x8C
#define KEY_F7				0x8D
#define KEY_F8				0x8E
#define KEY_F9				0x8F
#define KEY_F10				0x90
#define KEY_NUMLOCK			0x91
#define KEY_SCROLLLOCK		0x92
#define KEY_NUM_HOME		0x93
#define KEY_NUM_UP			0x94
#define KEY_NUM_PAGEUP		0x95
#define KEY_NUM_LEFT		0x96
#define KEY_NUM_CENTER		0x97
#define KEY_NUM_RIGHT		0x98
#define KEY_NUM_END			0x99
#define KEY_NUM_DOWN		0x9A
#define KEY_NUM_PAGEDOWN	0x9B
#define KEY_NUM_INS			0x9C
#define KEY_NUM_DEL			0x9D
#define KEY_F11				0x9E
#define KEY_F12				0x9F

/* 확장키 */
#define KEY_NUM_ENTER		0xA0
#define KEY_RCTRL			0xA1
#define KEY_NUM_SLASH		0xA2
#define KEY_RALT			0xA3
#define KEY_HOME			0xA4
#define KEY_UP				0xA5
#define KEY_PAGEUP			0xA6
#define KEY_LEFT			0xA7
#define KEY_RIGHT			0xA8
#define KEY_END				0xA9
#define KEY_DOWN			0xAA
#define KEY_PAGEDOWN		0xAB
#define KEY_INSERT			0xAC
#define KEY_DELETE			0xAD

#define KEY_PAUSE			0xAE

#define KEY_UNPUSH			0x100

#define KEY_NUM_MIN			0x47
#define KEY_NUM_MAX			0x53

#define KEY_CODE_PAUSE		0x91

#define SCAN_CODE_RSHIFT	0x36
#define SCAN_CODE_LSHIFT	0x2A

#endif // KeyTable_h_
