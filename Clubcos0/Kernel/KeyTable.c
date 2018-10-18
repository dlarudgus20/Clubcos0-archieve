#include "Keyboard.h"

KeyStruct g_KeyTable[] =
{
	/*	0x00	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x01	*/	{	KEY_ESC			,	KEY_ESC			, FALSE },
	/*	0x02	*/	{	'1'				,	'!'				, FALSE },
	/*	0x03	*/	{	'2'				,	'@'				, FALSE },
	/*	0x04	*/	{	'3'				,	'#'				, FALSE },
	/*	0x05	*/	{	'4'				,	'$'				, FALSE },
	/*	0x06	*/	{	'5'				,	'%'				, FALSE },
	/*	0x07	*/	{	'6'				,	'^'				, FALSE },
	/*	0x08	*/	{	'7'				,	'&'				, FALSE },
	/*	0x09	*/	{	'8'				,	'*'				, FALSE },
	/*	0x0A	*/	{	'9'				,	'('				, FALSE },
	/*	0x0B	*/	{	'0'				,	')'				, FALSE },
	/*	0x0C	*/	{	'-'				,	'_'				, FALSE },
	/*	0x0D	*/	{	'='				,	'+'				, FALSE },
	/*	0x0E	*/	{	KEY_BACKSPACE	,	KEY_BACKSPACE	, FALSE },
	/*	0x0F	*/	{	KEY_TAB			,	KEY_TAB			, FALSE },
	/*	0x10	*/	{	'q'				,	'Q'				, FALSE },
	/*	0x11	*/	{	'w'				,	'W'				, FALSE },
	/*	0x12	*/	{	'e'				,	'E'				, FALSE },
	/*	0x13	*/	{	'r'				,	'R'				, FALSE },
	/*	0x14	*/	{	't'				,	'T'				, FALSE },
	/*	0x15	*/	{	'y'				,	'Y'				, FALSE },
	/*	0x16	*/	{	'u'				,	'U'				, FALSE },
	/*	0x17	*/	{	'i'				,	'I'				, FALSE },
	/*	0x18	*/	{	'o'				,	'O'				, FALSE },
	/*	0x19	*/	{	'p'				,	'P'				, FALSE },
	/*	0x1A	*/	{	'['				,	'{'				, FALSE },
	/*	0x1B	*/	{	']'				,	'}'				, FALSE },
	/*	0x1C	*/	{	'\n'			,	'\n'			, FALSE },
	/*	0x1D	*/	{	KEY_LCTRL		,	KEY_LCTRL		, FALSE },
	/*	0x1E	*/	{	'a'				,	'A'				, FALSE },
	/*	0x1F	*/	{	's'				,	'S'				, FALSE },
	/*	0x20	*/	{	'd'				,	'D'				, FALSE },
	/*	0x21	*/	{	'f'				,	'F'				, FALSE },
	/*	0x22	*/	{	'g'				,	'G'				, FALSE },
	/*	0x23	*/	{	'h'				,	'H'				, FALSE },
	/*	0x24	*/	{	'j'				,	'J'				, FALSE },
	/*	0x25	*/	{	'k'				,	'K'				, FALSE },
	/*	0x26	*/	{	'l'				,	'L'				, FALSE },
	/*	0x27	*/	{	';'				,	':'				, FALSE },
	/*	0x28	*/	{	'\''			,	'\"'			, FALSE },
	/*	0x29	*/	{	'`'				,	'~'				, FALSE },
	/*	0x2A	*/	{	KEY_LSHIFT		,	KEY_LSHIFT		, FALSE },
	/*	0x2B	*/	{	'\\'			,	'|'				, FALSE },
	/*	0x2C	*/	{	'z'				,	'Z'				, FALSE },
	/*	0x2D	*/	{	'x'				,	'X'				, FALSE },
	/*	0x2E	*/	{	'c'				,	'C'				, FALSE },
	/*	0x2F	*/	{	'v'				,	'V'				, FALSE },
	/*	0x30	*/	{	'b'				,	'B'				, FALSE },
	/*	0x31	*/	{	'n'				,	'N'				, FALSE },
	/*	0x32	*/	{	'm'				,	'M'				, FALSE },
	/*	0x33	*/	{	','				,	'<'				, FALSE },
	/*	0x34	*/	{	'.'				,	'>'				, FALSE },
	/*	0x35	*/	{	'/'				,	'?'				, FALSE },
	/*	0x36	*/	{	KEY_RSHIFT		,	KEY_RSHIFT		, FALSE },
	/*	0x37	*/	{	'*'				,	'*'				, FALSE },
	/*	0x38	*/	{	KEY_LALT		,	KEY_LALT		, FALSE },
	/*	0x39	*/	{	' '				,	' '				, FALSE },
	/*	0x3A	*/	{	KEY_CAPSLOCK	,	KEY_CAPSLOCK	, FALSE },
	/*	0x3B	*/	{	KEY_F1			,	KEY_F1			, FALSE },
	/*	0x3C	*/	{	KEY_F2			,	KEY_F2			, FALSE },
	/*	0x3D	*/	{	KEY_F3			,	KEY_F3			, FALSE },
	/*	0x3E	*/	{	KEY_F4			,	KEY_F4			, FALSE },
	/*	0x3F	*/	{	KEY_F5			,	KEY_F5			, FALSE },
	/*	0x40	*/	{	KEY_F6			,	KEY_F6			, FALSE },
	/*	0x41	*/	{	KEY_F7			,	KEY_F7			, FALSE },
	/*	0x42	*/	{	KEY_F8			,	KEY_F8			, FALSE },
	/*	0x43	*/	{	KEY_F9			,	KEY_F9			, FALSE },
	/*	0x44	*/	{	KEY_F10			,	KEY_F10			, FALSE },
	/*	0x45	*/	{	KEY_NUMLOCK		,	KEY_NUMLOCK		, FALSE },
	/*	0x46	*/	{	KEY_SCROLLLOCK	,	KEY_SCROLLLOCK	, FALSE },
/*#define KEY_NUM_MIN 0x47 (Keyboard.h)*/
	/*	0x47	*/	{	KEY_NUM_HOME	,	'7'				, FALSE },
	/*	0x48	*/	{	KEY_NUM_UP		,	'8'				, FALSE },
	/*	0x49	*/	{	KEY_NUM_PAGEUP	,	'9'				, FALSE },
	/*	0x4A	*/	{	'-'				,	'-'				, FALSE },
	/*	0x4B	*/	{	KEY_NUM_LEFT	,	'4'				, FALSE },
	/*	0x4C	*/	{	KEY_NUM_CENTER	,	'5'				, FALSE },
	/*	0x4D	*/	{	KEY_NUM_RIGHT	,	'6'				, FALSE },
	/*	0x4E	*/	{	'+'				,	'+'				, FALSE },
	/*	0x4F	*/	{	KEY_NUM_END		,	'1'				, FALSE },
	/*	0x50	*/	{	KEY_NUM_DOWN	,	'2'				, FALSE },
	/*	0x51	*/	{	KEY_NUM_PAGEDOWN,	'3'				, FALSE },
	/*	0x52	*/	{	KEY_NUM_INS		,	'0'				, FALSE },
	/*	0x53	*/	{	KEY_NUM_DEL		,	'.'				, FALSE },
/*#define KEY_NUM_MAX 0x53 (Keyboard.h)*/
	/*	0x54	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x55	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x56	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },

	/*	0x57	*/	{	KEY_F11			,	KEY_F11			, FALSE },
	/*	0x58	*/	{	KEY_F12			,	KEY_F12			, FALSE },

	/* 확장키
	 * 확장키 공식 = (키 코드) - 0x1C + 0x59
	 */

	/*	0x59	*/	{	KEY_NUM_ENTER	,	KEY_ENTER		, FALSE }, /* 0xE0 0x1C */
	/*	0x5A	*/	{	KEY_RCTRL		,	KEY_RCTRL		, FALSE }, /* 0xE0 0x1D */

	/*	0x5B	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x5C	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x5D	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x5E	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x5F	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x60	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x61	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x62	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x63	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x64	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x65	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x66	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x67	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x68	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x69	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x6A	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x6B	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x6C	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x6D	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x6E	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x6F	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x70	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x71	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
		
	/*	0x72	*/	{	KEY_NUM_SLASH	,	'/'				, FALSE }, /* 0xE0 0x35 */

	/*	0x73	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x74	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },

	/*	0x75	*/	{	KEY_RALT		,	KEY_RALT		, FALSE }, /* 0xE0 0x38 */

	/*	0x73	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x74	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x75	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x76	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x77	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x78	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x79	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x7A	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x7B	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x7C	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x7D	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x7E	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x7F	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x80	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x81	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x82	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },
	/*	0x83	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },

	/*	0x84	*/	{	KEY_HOME		,	KEY_HOME		, FALSE }, /* 0xE0 0x47 */
	/*	0x85	*/	{	KEY_HOME		,	KEY_HOME		, FALSE }, /* 0xE0 0x48 */
	/*	0x86	*/	{	KEY_PAGEUP		,	KEY_PAGEUP		, FALSE }, /* 0xE0 0x49 */

	/*	0x87	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },

	/*	0x88	*/	{	KEY_LEFT		,	KEY_LEFT		, FALSE }, /* 0xE0 0x4B */

	/*	0x89	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },

	/*	0x8A	*/	{	KEY_RIGHT		,	KEY_RIGHT		, FALSE }, /* 0xE0 0x4D */

	/*	0x8B	*/	{	KEY_NONE		,	KEY_NONE		, FALSE },

	/*	0x8C	*/	{	KEY_END			,	KEY_END			, FALSE }, /* 0xE0 0x4F */
	/*	0x8D	*/	{	KEY_DOWN		,	KEY_DOWN		, FALSE }, /* 0xE0 0x50 */
	/*	0x8E	*/	{	KEY_PAGEDOWN	,	KEY_PAGEDOWN	, FALSE }, /* 0xE0 0x51 */
	/*	0x8F	*/	{	KEY_INSERT		,	KEY_INSERT		, FALSE }, /* 0xE0 0x52 */
	/*	0x90	*/	{	KEY_DELETE		,	KEY_DELETE		, FALSE }, /* 0xE0 0x53 */

/*#define KEY_CODE_PAUSE 0x91 (Keyboard.h)*/
	/*	0x91	*/	{	KEY_PAUSE		,	KEY_PAUSE		, FALSE }, /* 0xE1 0x1D 0x45 */
};
