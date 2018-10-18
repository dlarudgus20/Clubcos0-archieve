#ifndef Functions_h_
#define Functions_h_

#include "stdarg.h"

int isalpha(int c);
int isdigit(int c);

int strlen(const char *str);
void strcpy(char *des, const char *src);
void strrev(char *str);

void itoa(int i, char *buf, int radix);
void DecimalToString(int i, char *buf);
void HexaToString(int i, char *buf);

int sprintf(char *buf, const char *format, ...);
int vsprintf(char *buf, const char *format, va_list va);

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define range(a, x, b) (((x) < (a)) ? (a) : (min(x, b)))

#endif // Functions_h_
