#include "Clubcos0.h"
#include "DynArray.h"
#include "AssemblyFunc.h"

void InitDynArray32(DynArray32 *pArr, DWORD *start, DWORD *end)
{
	pArr->begin =  pArr->end = start;
	pArr->max_end = end;
}

DWORD *PushBackDynArray32(DynArray32 *pArr, DWORD data)
{
	if (pArr->end != pArr->max_end)
	{
		*pArr->end = data;
		return pArr->end++;
	}
	else
	{
		return pArr->max_end;
	}
}

BOOL EraseDynArray32(DynArray32 *pArr, DWORD *ptr)
{
	if (pArr->begin <= ptr && pArr->end > ptr)
	{
		memcpy(ptr, ptr + 1, (pArr->end - ptr - 1) * sizeof(DWORD));
		pArr->end--;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
