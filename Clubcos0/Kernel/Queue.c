#include "Clubcos0.h"
#include "Queue.h"

void InitQueue32(Queue32 *pQueue, DWORD *pQueueBuf, DWORD QueueSize)
{
	pQueue->pQueueBuf = pQueueBuf;
	pQueue->pFirst = pQueue->pLast = pQueueBuf;
	pQueue->size = QueueSize;
	pQueue->bEmpty = TRUE;
}

BOOL PutQueue32(Queue32 *pQueue, DWORD data)
{
	if(pQueue->bEmpty || (pQueue->pFirst != pQueue->pLast))
	{
		*pQueue->pLast = data;
		pQueue->pLast++;

		if(pQueue->pLast >= &pQueue->pQueueBuf[pQueue->size])
		{
			pQueue->pLast = pQueue->pQueueBuf;
		}

		pQueue->bEmpty = FALSE;

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

DWORD GetQueue32(Queue32 *pQueue, BOOL bPeek, BOOL *pbSuccess)
{
	DWORD nRet;
	BOOL b;

	if (pbSuccess == NULL)
		pbSuccess = &b;

	if(!pQueue->bEmpty)
	{
		nRet = *pQueue->pFirst;

		if (!bPeek)
			RemoveQueue32(pQueue);

		*pbSuccess = TRUE;
		return nRet;
	}
	else
	{
		*pbSuccess = FALSE;
		return 0;
	}
}

BOOL RemoveQueue32(Queue32 *pQueue)
{
	if (!pQueue->bEmpty)
	{
		pQueue->pFirst++;
	
		if(pQueue->pFirst >= &pQueue->pQueueBuf[pQueue->size])
		{
			pQueue->pFirst = pQueue->pQueueBuf;
		}
	
		if(pQueue->pFirst == pQueue->pLast)
		{
			pQueue->bEmpty = TRUE;
		}

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

