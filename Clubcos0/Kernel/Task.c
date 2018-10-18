#include "Clubcos0.h"
#include "Task.h"
#include "AssemblyFunc.h"
#include "Memory.h"
#include "Timer.h"

TaskStruct g_TaskStruct;

static void IdleTask();

static inline BOOL CheckIsTaskPtr(Task *pTask)
{
	return (g_TaskStruct.tasks <= pTask && &g_TaskStruct.tasks[MAX_TASK] > pTask);
}

void InitTss(Tss *pTss, WORD ds, WORD cs, DWORD eip, WORD ss, DWORD esp)
{
	pTss->eip = eip;
	pTss->eflags = EFLAG_IF | EFLAG_2;

	pTss->eax = pTss->ecx = pTss->edx = pTss->ebx = pTss->esi = pTss->edi = 0;

	pTss->ebp = pTss->esp = esp;

	pTss->es = pTss->ds = pTss->ds = pTss->fs = pTss->gs = ds;
	pTss->cs = cs;
	pTss->ss = ss;

	pTss->ldtr = 0;
	pTss->iomap = 0x40000000;
}

void InitTssDescriptor(Gdt *pGdt, Tss *pTss, BYTE dpl)
{
	pGdt->Size_0_15 = 0x68;
	pGdt->Size_16_19 = 0;
	pGdt->G = 0;

	pGdt->Address_0_15 = (WORD)(DWORD)pTss;
	pGdt->Address_16_23 = (BYTE)((DWORD)pTss >> 16);
	pGdt->Address_24_32 = (BYTE)((DWORD)pTss >> 24);

	pGdt->SegmentType = 0x9;
	pGdt->S = 0;
	pGdt->DPL = dpl;
	pGdt->P = 1;

	pGdt->reserved = pGdt->D = 0;
}

static inline void InitTaskArray(TaskArray *pArr)
{
	InitDynArray32(&pArr->arpTask, (DWORD *)pArr->pTask_raw,
			(DWORD *)(pArr->pTask_raw + MAX_TASK));
}
void InitTaskStruct()
{
	DynArray32 *pArr;
	void *IdleStack;
	int i;

	memset(g_TaskStruct.tasks, 0, sizeof(g_TaskStruct.tasks));
	memset(g_TaskStruct.ExecuteCount, 0, sizeof(g_TaskStruct.ExecuteCount));

	for (i = 0; i < COUNT_TASK_PRIORITY; i++)
	{
		InitTaskArray(&g_TaskStruct.ReadyList[i]);
	}

	InitTaskArray(&g_TaskStruct.WaitForExitList);

	/* Kernel Task Init */
	g_TaskStruct.tasks[0].selector = TASK_GDT_0;
	g_TaskStruct.tasks[0].flags = TASK_RUNNING;
	g_TaskStruct.tasks[0].priority = KERNEL_TASK_PRIORITY;
	g_TaskStruct.ExecuteCount[KERNEL_TASK_PRIORITY] = 1;

	InitTssDescriptor(((Gdt *)GDT_ADDRESS) + TASK_GDT_0, &g_TaskStruct.tasks[0].tss, 0);

	g_TaskStruct.pNow = &g_TaskStruct.tasks[0];
	LoadTr(TASK_GDT_0 * 8);

	/* Idle Task Init */
	g_TaskStruct.tasks[1].selector = TASK_GDT_0 + 1;
	g_TaskStruct.tasks[1].flags = TASK_READY;
	g_TaskStruct.tasks[1].priority = TASK_PRIORITY_IDLE;
	g_TaskStruct.ExecuteCount[TASK_PRIORITY_IDLE] = 0;

	IdleStack = AllocateMemory(2 * 1024 * 1024);
	InitTss(&g_TaskStruct.tasks[1].tss,
		KERNEL_DATA_SEGMENT, KERNEL_CODE_SEGMENT,
		(DWORD)IdleTask, KERNEL_DATA_SEGMENT, (DWORD)IdleStack);
	InitTssDescriptor(((Gdt *)GDT_ADDRESS) + TASK_GDT_0 + 1,
		&g_TaskStruct.tasks[1].tss, 0);

	pArr = &g_TaskStruct.ReadyList[TASK_PRIORITY_IDLE].arpTask;
	PushBackDynArray32(pArr, (DWORD)&g_TaskStruct.tasks[1]);

	/* Data for Measure Processor Time Init */
	g_TaskStruct.IdleProcessTime = 0;
}

Task *CreateTask(DWORD eip, DWORD esp, TaskPriority priority)
{
	DynArray32 *pArr = &g_TaskStruct.ReadyList[priority].arpTask;
	Task *ret = NULL;
	DWORD eflag;
	int i;

	eflag = GetEFlag();
	AsmCli();

	if (pArr->end != pArr->max_end)
	{
		for (i = 2; i < MAX_TASK; i++)
		{
			if (g_TaskStruct.tasks[i].selector == 0)
			{
				if (MaxSizeDynArray32(pArr) - SizeDynArray32(pArr)
						> (g_TaskStruct.pNow->priority == priority) ? 1 : 0)
				{
					ret = &g_TaskStruct.tasks[i];

					ret->selector = TASK_GDT_0 + i;
					ret->flags = TASK_READY;
					ret->priority = priority;
					InitTss(&ret->tss, KERNEL_DATA_SEGMENT, KERNEL_CODE_SEGMENT,
							eip, KERNEL_DATA_SEGMENT, esp);
					InitTssDescriptor(((Gdt *)GDT_ADDRESS) + TASK_GDT_0 + i, &ret->tss, 0);

					PushBackDynArray32(pArr, (DWORD)ret);
				}

				break;
			}
		}
	}

	SetEFlag(eflag);
	return ret;
}

BOOL TerminateTask(Task *pTask)
{
	DynArray32 *pArr;
	DWORD *pFound;

	BOOL bRet = FALSE;
	DWORD eflag;

	if (!CheckIsTaskPtr(pTask))
		return FALSE;

	if (pTask > &g_TaskStruct.tasks[1])	// System (0), Idle (1) Task
	{
		eflag = GetEFlag();
		AsmCli();

		if (pTask == g_TaskStruct.pNow)
		{
			pTask->flags = TASK_WAITFOREXIT;
			SetEFlag(eflag);while(1);//TaskSchedule();
			/* Exit itself - No Return */
		}
		else
		{
			pArr = &g_TaskStruct.ReadyList[pTask->priority].arpTask;
			if (RemoveDynArray32(pArr, (DWORD)pTask) == pArr->end)
			{	/* waiting 상태일 때.. 아직 구현 안 됌 */
				bRet = FALSE;
				goto Err;
			}

			pTask->flags = TASK_WAITFOREXIT;
			PushBackDynArray32(&g_TaskStruct.WaitForExitList.arpTask, (DWORD)pTask);
		}
Err:
		SetEFlag(eflag);
	}
	return bRet;
}

BOOL ChangePriorityTask(Task *pTask, TaskPriority priority)
{
	DWORD eflag;
	BOOL bRet = FALSE;

	if (!CheckIsTaskPtr(pTask))
		return FALSE;

	eflag = GetEFlag();
	AsmCli();

	if (pTask == g_TaskStruct.pNow)
	{
		pTask->priority = priority;
		bRet = TRUE;
	}
	else
	{
		DynArray32 *pTargetArr = &g_TaskStruct.ReadyList[priority].arpTask;
		DynArray32 *pSrcArr = &g_TaskStruct.ReadyList[pTask->priority].arpTask;

		if (RemoveDynArray32(pSrcArr, (DWORD)pTask) != pSrcArr->end)
		{
			PushBackDynArray32(pTargetArr, (DWORD)pTask);
			pTask->priority = priority;
			bRet = TRUE;
		}
	}

	SetEFlag(eflag);
	return bRet;
}
#include "Graphic.h"
void TaskSchedule()
{
	Task *pNow = NULL;
	int i, j;

	DWORD eflag;

	eflag = GetEFlag();
	AsmCli();

	for (j = 0; j < 2; j++)
	{
		for (i = 0; i < COUNT_TASK_PRIORITY; i++)
		{
			if (g_TaskStruct.ExecuteCount[i] <
				SizeDynArray32(&g_TaskStruct.ReadyList[i].arpTask))
			{
				pNow = (Task *)PopFrontDynArray32(&g_TaskStruct.ReadyList[i].arpTask);
				g_TaskStruct.ExecuteCount[i]++;
				break;
			}
			else
			{
				g_TaskStruct.ExecuteCount[i] = 0;
			}
		}

		if (pNow != NULL)
			break;
	}

	if (pNow != NULL)
	{
		if (g_TaskStruct.pNow->flags == TASK_WAITFOREXIT)
		{
			PushBackDynArray32(&g_TaskStruct.WaitForExitList.arpTask, (DWORD)g_TaskStruct.pNow);
		}
		else
		{
			PushBackDynArray32(&g_TaskStruct.ReadyList[g_TaskStruct.pNow->priority].arpTask,
				(DWORD)g_TaskStruct.pNow);

			g_TaskStruct.pNow->flags = TASK_READY;
		}

		if (pNow->selector == 4) /* Idle Task */
		{
			g_TaskStruct.IdleProcessTime++;
		}

		g_TaskStruct.pNow = pNow;
		g_TaskStruct.pNow->flags = TASK_RUNNING;

		FarJmp(0, g_TaskStruct.pNow->selector * 8);
	}

	SetEFlag(eflag);
}

static void IdleTask()
{
	DWORD ProcessorTick, LastProcessorTick = g_Timer.TickCountLow;
	Task *pTask;

	while (1)
	{
		ProcessorTick = g_Timer.TickCountLow;

		if (ProcessorTick - LastProcessorTick == 0)
		{
			g_TaskStruct.ProcessorLoad = 100;
		}
		else
		{
			g_TaskStruct.ProcessorLoad = 100 -
					(g_TaskStruct.IdleProcessTime * TASK_QUANTUM * 100)
					/ (ProcessorTick - LastProcessorTick);
		}

		while (SizeDynArray32(&g_TaskStruct.WaitForExitList.arpTask) > 0)
		{
			pTask = (Task *)PopFrontDynArray32(&g_TaskStruct.WaitForExitList.arpTask);
			InitNullGdt(((Gdt *)GDT_ADDRESS) + pTask->selector);
			pTask->selector = 0;
		}

		TaskSchedule();
	}
}
