#ifndef Task_h_
#define Task_h_

#include "Gdt.h"
#include "DynArray.h"

#define TASK_QUANTUM 20

#define MAX_TASK 1024

#define TASK_GDT_0 3

typedef enum tagTaskPriority
{
	TASK_PRIORITY_REALTIME,
	TASK_PRIORITY_HIGHEST,
	TASK_PRIORITY_HIGH,
	TASK_PRIORITY_ABOVE_NORMAL,
	TASK_PRIORITY_NORMAL,
	TASK_PRIORITY_BELOW_NORMAL,
	TASK_PRIORITY_LOW,
	TASK_PRIORITY_LOWEST,
	TASK_PRIORITY_IDLE,

	COUNT_TASK_PRIORITY,
	MAX_TASK_PRIORITY = COUNT_TASK_PRIORITY - 1,

	KERNEL_TASK_PRIORITY = TASK_PRIORITY_HIGHEST
} TaskPriority;

typedef struct tagTss
{
	DWORD backlink, esp0, ss0, esp1, ss1, esp2, ss2, cr3;
	DWORD eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
	DWORD es, cs, ss, ds, fs, gs;
	DWORD ldtr, iomap;
} Tss;

typedef enum tagTaskFlag
{
	TASK_RUNNING,
	TASK_READY,
	TASK_WAIT,
	TASK_WAITFOREXIT
} TaskFlag;

typedef struct tagTask
{
	DWORD selector;

	TaskFlag flags:2;
	TaskPriority priority:30;

	Tss tss;
} Task;

typedef struct tagTaskArray
{
	DynArray32 arpTask;
	Task *pTask_raw[MAX_TASK];
} TaskArray;

typedef struct tagTaskStruct
{
	Task tasks[MAX_TASK];

	TaskArray ReadyList[COUNT_TASK_PRIORITY];
	TaskArray WaitForExitList;

	DWORD ExecuteCount[COUNT_TASK_PRIORITY];
	Task *pNow;

	DWORD IdleProcessTime;
	DWORD ProcessorLoad;
} TaskStruct;

extern TaskStruct g_TaskStruct;

void InitTss(Tss *pTss, WORD ds, WORD cs, DWORD eip, WORD ss, DWORD esp);
void InitTssDescriptor(Gdt *pGdt, Tss *pTss, BYTE dpl);

void InitTaskStruct();

Task *CreateTask(DWORD eip, DWORD esp, TaskPriority priority);
BOOL TerminateTask(Task *pTask);
static inline void ExitTask()
	{ TerminateTask(g_TaskStruct.pNow); }

BOOL ChangePriorityTask(Task *pTask, TaskPriority priority);

static inline Task *GetCurrentTask()
	{ return g_TaskStruct.pNow; }

static inline DWORD GetTaskId(Task *pTask)
	{ return pTask - g_TaskStruct.tasks; }

void TaskSchedule();

#endif // Task_h_
