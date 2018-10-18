#ifndef KernelData_h_
#define KernelData_h_

#include "Clubcos0.h"
#include "Queue.h"

#define INTERRUPT_QUEUE_SIZE 512

typedef struct tagKernelData
{
	Queue32 InterruptQueue;
} KernelData;

extern KernelData g_kData;

void InitKernelData();

#endif // KernelData_h_
