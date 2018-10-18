#include "Clubcos0.h"
#include "KernelData.h"
#include "Queue.h"

KernelData g_kData;

static DWORD InterruptQueueBuf[INTERRUPT_QUEUE_SIZE];

void InitKernelData()
{
	InitQueue32(&g_kData.InterruptQueue, InterruptQueueBuf, INTERRUPT_QUEUE_SIZE);
}
