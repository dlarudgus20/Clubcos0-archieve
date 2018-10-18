#ifndef Graphic_h_
#define Graphic_h_

#include "Clubcos0.h"

#define RGB(r, g, b) ((((r) << 11) | ((g) << 5) | (b)) & 0xffff)
											/* RGB�κ��� �� �ڵ带 ��� ��ũ�� */

#define RGB_MAX 0x1f						/* RED�� BLUE�� �ִ밪 */
#define GREEN_MAX 0x3f						/* GREEN�� �ִ밪 */

/* RGB ��� ���� */
#define RGB_LEVEL1 (RGB_MAX*4/4)
#define RGB_LEVEL2 (RGB_MAX*3/4)
#define RGB_LEVEL3 (RGB_MAX*2/4)
#define RGB_LEVEL4 (RGB_MAX*1/4)

/* GREEN ��� ���� */
#define GREEN_LEVEL1 (GREEN_MAX*4/4)
#define GREEN_LEVEL2 (GREEN_MAX*3/4)
#define GREEN_LEVEL3 (GREEN_MAX*2/4)
#define GREEN_LEVEL4 (GREEN_MAX*1/4)

/* ����(RED) �迭 (SCARLET = ��ȫ��) */
#define RED					RGB(RGB_LEVEL1,0,0)
#define BRIGHT_SCARLET		RGB(RGB_LEVEL2,0,0)
#define SCARLET				RGB(RGB_LEVEL3,0,0)
#define DARK_SCARLET		RGB(RGB_LEVEL4,0,0)

/* �ʷϻ�(LIGNTGREEN) �迭 (���Ѱ� �׳� GREEN(���)) */
#define LIGHTGREEN			RGB(0,GREEN_LEVEL1,0)
#define BRIGHT_GRREN		RGB(0,GREEN_LEVEL2,0)
#define GREEN				RGB(0,GREEN_LEVEL3,0)
#define DARK_GREEN			RGB(0,GREEN_LEVEL4,0)

/* �Ķ���(BLUE) �迭 (NAVY = ����) */
#define BLUE				RGB(0,0,RGB_LEVEL1)
#define BRIGHT_NAVY			RGB(0,0,RGB_LEVEL2)
#define NAVY				RGB(0,0,RGB_LEVEL3)
#define DARK_NAVY			RGB(0,0,RGB_LEVEL4)

/* �����(YELLOW) �迭 (DEEPYELLOW = ���� �����) */
#define YELLOW				RGB(RGB_LEVEL1,GREEN_LEVEL1,0)
#define DARK_DEEPYELLOW		RGB(RGB_LEVEL2,GREEN_LEVEL2,0)
#define DEEPYELLOW			RGB(RGB_LEVEL3,GREEN_LEVEL3,0)
#define BRIGHT_DEEPYELLOW	RGB(RGB_LEVEL4,GREEN_LEVEL4,0)

/* ����Ÿ(���� ���ֻ�:MAGENTA) �迭 (PLUM = ���ֻ�) */
#define MAGENTA				RGB(RGB_LEVEL1,0,RGB_LEVEL1)
#define DARK_PLUM			RGB(RGB_LEVEL2,0,RGB_LEVEL2)
#define PLUM				RGB(RGB_LEVEL3,0,RGB_LEVEL3)
#define BRIGHT_PLUM			RGB(RGB_LEVEL4,0,RGB_LEVEL4)

/* �þ�(���� û�ϻ�) �迭 (BLUEGREEN = û�ϻ�) */
#define CYAN				RGB(0,GREEN_LEVEL1,RGB_LEVEL1)
#define DARK_BLUEGREEN		RGB(0,GREEN_LEVEL2,RGB_LEVEL2)
#define BLUEGREEN			RGB(0,GREEN_LEVEL3,RGB_LEVEL3)
#define BRIGHT_BLUEGREEN	RGB(0,GREEN_LEVEL4,RGB_LEVEL4)

/* ���(WHITE) �迭 (GRAY = ȸ��) */
#define WHITE				RGB(RGB_LEVEL1,GREEN_LEVEL1,RGB_LEVEL1)
#define BRIGHT_GRAY			RGB(RGB_LEVEL2,GREEN_LEVEL2,RGB_LEVEL2)
#define GRAY				RGB(RGB_LEVEL3,GREEN_LEVEL3,RGB_LEVEL3)
#define DARK_GRAY			RGB(RGB_LEVEL4,GREEN_LEVEL4,RGB_LEVEL4)

#define BLACK				RGB(0,0,0)

/* Clubcos0�� ����ϴ� ����� */
#define BACK_COLOR DARK_BLUEGREEN

/* ������ ��Ÿ���� Ÿ�� (16bit = WORD) */
typedef WORD COLOR;

/* BootInfo */
typedef struct tagBootInfo
{
	BYTE ReadCylinders;							/* READ_CYLS : �о���� �Ǹ��� �� */
	WORD ScreenX;								/* SCREEN_X : ȭ���� X��ǥ ũ�� */
	WORD ScreenY;								/* SCREEN_Y : ȭ���� X��ǥ ũ�� */
	COLOR *VideoMemory;							/* VIDEO_MEMORY : ���� �޸��� ������ */
} BootInfo;

static inline BootInfo *GetBootInfo() { return (BootInfo *)0x268000; }

#define PRINT_XY(X, Y) ((X) + ((Y) * GetBootInfo()->ScreenX))

void DrawBoxFill(int x1, int y1, int x2, int y2, COLOR color);
void DrawBox(int x1, int y1, int x2, int y2, COLOR color);
void DrawPixel(int x, int y, COLOR color);

void DrawBackGround();

void DrawFont(const unsigned char *font, int x, int y, COLOR c);
void DrawString(const char *str, int x, int y, COLOR c);
void DrawStringFormat(int x, int y, COLOR c, const char *format, ...);

void DrawTaskBar(DWORD height, DWORD StartButtonMargin, DWORD StartButtonWidth);
void DrawButton(int x1, int y1, int x2, int y2, COLOR color);

void DrawBlock(const char *block, int x, int y, int width, int height, COLOR BackColor);

extern unsigned char g_vucEnglishFont[];

#endif // Graphic_h_
