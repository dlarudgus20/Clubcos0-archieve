#include "Clubcos0.h"
#include "Functions.h"

int isalpha(int c)
{
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

int isdigit(int c)
{
	return c >= '0' && c <= '9';
}

int strlen(const char *str)
{
	int i;

	for(i = 0; str[i] != 0; i++) { }

	return i;
}

void strcpy(char *des, const char *src)
{
	int i;

	for(i = 0; src[i] != 0; i++)
	{
		des[i] = src[i];
	}

	des[i] = NULL;
}

void strrev(char *str)
{
	int i, j;
	char tmp;

	j = strlen(str) - 1;
	for(i = 0; str[i] != NULL; i++)
	{
		if(!(i < j))
		{
			break;
		}
		tmp = str[i];
		str[i] = str[j];
		str[j] = tmp;
		j--;
	}
}


void itoa(int i, char *buf, int radix)
{
	switch(radix)
	{
	case 10:
	default:
		DecimalToString(i, buf);
		break;
	case 16:
		HexaToString(i, buf);
	}
}

void DecimalToString(int i, char *buf)
{
	int index = 0;

	if(i == 0)
	{
		strcpy(buf, "0");
		return;
	}
	else if(i == -2147483648)
	{
		strcpy(buf, "-2147483648");
		return;
	}
	else if(i < 0)
	{
		buf[0] = '-';
		index = 1;
		i = -i;
	}

	for(; i > 0; index++)
	{
		buf[index] = '0' + i % 10;
		i /= 10;
	}
	buf[index] = NULL;

	if(buf[0] == '-')
	{
		strrev(&buf[1]);
	}
	else
	{
		strrev(buf);
	}
}

void HexaToString(int i, char *buf)
{
	unsigned u = (unsigned) i;
	int index = 0;

	if(u == 0)
	{
		strcpy(buf, "0");
		return;
	}

	for(; u > 0; index++)
	{
		if((u % 16) <= 9)
		{
			buf[index] = '0' + u % 16;
		}
		else
		{
			buf[index] = 'a' + (u % 16 - 0x0a);
		}
		u /= 16;
	}
	buf[index] = NULL;

	if(buf[0] == '-')
	{
		strrev(&buf[1]);
	}
	else
	{
		strrev(buf);
	}
}

int sprintf(char *buf, const char *format, ...)
{
	va_list va;
	int ret;

	va_start(va, format);
	ret = vsprintf(buf, format, va);
	va_end(va);

	return ret;
}

int vsprintf(char *buf, const char *format, va_list va)
{
	int FormatIdx, BufIdx = 0;

	for (FormatIdx = 0; format[FormatIdx] != '\0'; FormatIdx++)
	{
		if (format[FormatIdx] == '%')
		{
			FormatIdx++;
			switch (format[FormatIdx])
			{
			case 'd':
			case 'i':
				itoa(va_arg(va, int), &buf[BufIdx], 10);
				BufIdx = strlen(buf);
				break;
			case 'x':
			case 'X':
			case 'p':
				itoa(va_arg(va, int), &buf[BufIdx], 16);
				BufIdx = strlen(buf);
				break;
			case 's':
				strcpy(&buf[BufIdx], va_arg(va, const char *));
				BufIdx = strlen(buf);
				break;
			case 'c':
				buf[BufIdx] = (char)va_arg(va, int);
				BufIdx++;
				break;
			default:
				buf[BufIdx] = format[FormatIdx];
				BufIdx++;
				break;
			}
		}
		else
		{
			buf[BufIdx] = format[FormatIdx];
			BufIdx++;
		}
	}

	buf[BufIdx] = NULL;
	return BufIdx;
}
