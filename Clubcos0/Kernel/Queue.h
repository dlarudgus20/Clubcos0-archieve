#ifndef Queue_h_
#define Queue_h_

#include "Clubcos0.h"

typedef struct tagQueue32
{
	DWORD *pQueueBuf;
	DWORD *pFirst, *pLast;
	DWORD size;
	BOOL bEmpty;
} Queue32;

void InitQueue32(Queue32 *pQueue, DWORD *pQueueBuf, DWORD QueueSize);

BOOL PutQueue32(Queue32 *pQueue, DWORD data);
DWORD GetQueue32(Queue32 *pQueue, BOOL bPeek, BOOL *pbSuccess);

BOOL RemoveQueue32(Queue32 *pQueue);

#endif // Queue_h_
