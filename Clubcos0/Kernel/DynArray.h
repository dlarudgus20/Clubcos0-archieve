#ifndef DynArray_h_
#define DynArray_h_

#include "AssemblyFunc.h"

typedef struct tagDynArray32
{
	DWORD *begin, *end;
	DWORD *max_end;
} DynArray32;

void InitDynArray32(DynArray32 *pArr, DWORD *start, DWORD *end);

DWORD *PushBackDynArray32(DynArray32 *pArr, DWORD data);
BOOL EraseDynArray32(DynArray32 *pArr, DWORD *ptr);

static inline DWORD *FindDynArray32(DynArray32 *pArr, DWORD data)
{
	return (DWORD *)memchr_4(pArr->begin, data, pArr->end - pArr->begin);
}

static inline DWORD *RemoveDynArray32(DynArray32 *pArr, DWORD data)
{
	DWORD *pFound = FindDynArray32(pArr, data);
	if (pFound != pArr->end)
	{
		EraseDynArray32(pArr, pFound);
	}
	return pFound;
}

static inline DWORD SizeDynArray32(DynArray32 *pArr)
{
	return pArr->end - pArr->begin;
}

static inline DWORD MaxSizeDynArray32(DynArray32 *pArr)
{
	return pArr->max_end - pArr->begin;
}

static inline DWORD PopFrontDynArray32(DynArray32 *pArr)
{
	DWORD ret = *pArr->begin;
	EraseDynArray32(pArr, pArr->begin);
	return ret;
}

#endif // DynArray_h_
