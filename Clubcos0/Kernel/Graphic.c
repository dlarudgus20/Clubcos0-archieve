#include "Clubcos0.h"
#include "Graphic.h"
#include "Functions.h"
#include "AssemblyFunc.h"

#define va_start(v,l)	__builtin_va_start((v),l)
#define va_end			__builtin_va_end
#define va_arg			__builtin_va_arg
#define va_copy(d,s)	__builtin_va_copy((d),(s))
#define	va_list			__builtin_va_list

void DrawBoxFill(int x1, int y1, int x2, int y2, COLOR color)
{
	BootInfo *pbi = GetBootInfo();
	int y;

	for (y = y1; y <= y2; y++)
	{
		memset_2(&pbi->VideoMemory[pbi->ScreenX * y + x1], color, (x2 - x1) * 2);
	}
}

void DrawBox(int x1, int y1, int x2, int y2, COLOR color)
{
	DrawBoxFill(x1, y1, x2, y1, color);
	DrawBoxFill(x2, y1, x2, y2, color);
	DrawBoxFill(x1, y2, x2, y2, color);
	DrawBoxFill(x1, y1, x1, y2, color);
}

void DrawPixel(int x, int y, COLOR color)
{
	GetBootInfo()->VideoMemory[PRINT_XY(x, y)] = color;
}

void DrawTaskBar(DWORD height, DWORD StartButtonMargin, DWORD StartButtonWidth)
{
	DrawBoxFill(0, GetBootInfo()->ScreenY - height,
		GetBootInfo()->ScreenX, GetBootInfo()->ScreenY, BRIGHT_GRAY);

	DrawBoxFill(0, GetBootInfo()->ScreenY - height,
		GetBootInfo()->ScreenX, GetBootInfo()->ScreenY - height, GRAY);

	DrawButton(StartButtonMargin, GetBootInfo()->ScreenY - (height - StartButtonMargin),
		StartButtonMargin + StartButtonWidth, GetBootInfo()->ScreenY - StartButtonMargin, GRAY);
}

void DrawButton(int x1, int y1, int x2, int y2, COLOR color)
{
	DrawBoxFill(x1, y1, x2, y2, color);
	DrawBox(x1, y1, x2, y2, DARK_GRAY);
}

void DrawBackGround()
{
	DrawBoxFill(0, 0, GetBootInfo()->ScreenX, GetBootInfo()->ScreenY, BACK_COLOR);
}

void DrawFont(const unsigned char *font, int x, int y, COLOR c)
{
	int i;

	for(i = 0; i < 16; i++)
	{
		if((font[i] & 0x80) != 0) DrawPixel(x + 0, y + i, c);
		if((font[i] & 0x40) != 0) DrawPixel(x + 1, y + i, c);
		if((font[i] & 0x20) != 0) DrawPixel(x + 2, y + i, c);
		if((font[i] & 0x10) != 0) DrawPixel(x + 3, y + i, c);
		if((font[i] & 0x08) != 0) DrawPixel(x + 4, y + i, c);
		if((font[i] & 0x04) != 0) DrawPixel(x + 5, y + i, c);
		if((font[i] & 0x02) != 0) DrawPixel(x + 6, y + i, c);
		if((font[i] & 0x01) != 0) DrawPixel(x + 7, y + i, c);
	}
}

void DrawString(const char *str, int x, int y, COLOR c)
{
	int i;

	for(i = 0; str[i] != NULL; i++)
	{
		DrawFont(&g_vucEnglishFont[str[i] * 16], x + (i * 8), y, c);
	}
}

void DrawStringFormat(int x, int y, COLOR c, const char *format, ...)
{
	char buf[1024];
	va_list va;

	va_start(va, format);
	vsprintf(buf, format, va);
	DrawString(buf, x, y, c);
	va_end(va);
}

void DrawBlock(const char *block, int x, int y, int width, int height, COLOR BackColor)
{
	int px, py;
	char NowBlock;

	for(px = 0; px < width; px++)
	{
		for(py = 0; py < height; py++)
		{
			NowBlock = block[px + (py * width)];
			if(NowBlock == '*')
			{
				DrawPixel((x + px), (y + py), BLACK);
			}
			else if(NowBlock == '-')
			{
				DrawPixel((x + px), (y + py), BRIGHT_GRAY);
			}
			else if(NowBlock == '/')
			{
				DrawPixel((x + px), (y + py), GRAY);
			}
			else if(NowBlock == '@')
			{
				DrawPixel((x + px), (y + py), DARK_GRAY);
			}
			else if(NowBlock == 'o')
			{
				DrawPixel((x + px), (y + py), WHITE);
			}
			else if(NowBlock == '.')
			{
				DrawPixel((x + px), (y + py), BackColor);
			}
		}
	}
}
