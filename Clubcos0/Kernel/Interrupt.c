#include "Clubcos0.h"
#include "Interrupt.h"
#include "Graphic.h"

int _DefaultHandler(DWORD id, InterruptContext *esp)
{
	DrawStringFormat(0, 700, BLACK, "Interrupt Occur?!?! [%d]", id);
	while (1);
}
