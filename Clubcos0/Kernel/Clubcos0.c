#include "Clubcos0.h"
#include "Graphic.h"
#include "Functions.h"
#include "GDT.h"
#include "IDT.h"
#include "Interrupt.h"
#include "PIC.h"
#include "Keyboard.h"
#include "AssemblyFunc.h"
#include "KernelData.h"
#include "Queue.h"
#include "Mouse.h"
#include "Memory.h"
#include "Timer.h"
#include "Task.h"

void ClcMain()
{
	Gdt *pGdtTable = (Gdt *)GDT_ADDRESS;
	Idt *pIdtTable = (Idt *)IDT_ADDRESS;
	DWORD QueueData;
	int i;

	/* 배경화면 그리기 */
	DrawBackGround();
	DrawTaskBar(30, 5, 60);

	/* GDT 초기화 */
	for(i = 0; i < 8192; i++)
	{
		InitNullGdt(&pGdtTable[i]);
	}
	InitGdt(&pGdtTable[1], 0x00000000, 0xffffffff, MAKE_GDT_TYPE1(CODE_SEGMENT_TYPE, 1, 0, 1));
	InitGdt(&pGdtTable[2], 0x00000000, 0xffffffff, MAKE_GDT_TYPE1(DATA_SEGMENT_TYPE, 1, 0, 1));

	LoadGdt(0xffff, pGdtTable);

	/* IDT 초기화 */
	for(i = 0; i < 256; i++)
	{
		InitIdt(&pIdtTable[i], NULL, NULL_SEGMENT, 0);
	}

	LoadIdt(256 * sizeof(Idt), pIdtTable);

	InitIdt(&pIdtTable[0], DefaultHandler0, KERNEL_CODE_SEGMENT, 0);
	InitIdt(&pIdtTable[1], DefaultHandler1, KERNEL_CODE_SEGMENT, 0);
	InitIdt(&pIdtTable[2], DefaultHandler2, KERNEL_CODE_SEGMENT, 0);
	InitIdt(&pIdtTable[3], DefaultHandler3, KERNEL_CODE_SEGMENT, 0);
	InitIdt(&pIdtTable[4], DefaultHandler4, KERNEL_CODE_SEGMENT, 0);
	InitIdt(&pIdtTable[5], DefaultHandler5, KERNEL_CODE_SEGMENT, 0);
	InitIdt(&pIdtTable[6], DefaultHandler6, KERNEL_CODE_SEGMENT, 0);
	InitIdt(&pIdtTable[7], DefaultHandler7, KERNEL_CODE_SEGMENT, 0);
	InitIdt(&pIdtTable[8], DefaultHandler8, KERNEL_CODE_SEGMENT, 0);
	InitIdt(&pIdtTable[9], DefaultHandler9, KERNEL_CODE_SEGMENT, 0);
	InitIdt(&pIdtTable[10], DefaultHandler10, KERNEL_CODE_SEGMENT, 0);
	InitIdt(&pIdtTable[11], DefaultHandler11, KERNEL_CODE_SEGMENT, 0);
	InitIdt(&pIdtTable[12], DefaultHandler12, KERNEL_CODE_SEGMENT, 0);
	InitIdt(&pIdtTable[13], DefaultHandler13, KERNEL_CODE_SEGMENT, 0);
	InitIdt(&pIdtTable[14], DefaultHandler14, KERNEL_CODE_SEGMENT, 0);
	InitIdt(&pIdtTable[15], DefaultHandler15, KERNEL_CODE_SEGMENT, 0);
	InitIdt(&pIdtTable[16], DefaultHandler16, KERNEL_CODE_SEGMENT, 0);
	InitIdt(&pIdtTable[17], DefaultHandler17, KERNEL_CODE_SEGMENT, 0);
	InitIdt(&pIdtTable[18], DefaultHandler18, KERNEL_CODE_SEGMENT, 0);
	InitIdt(&pIdtTable[19], DefaultHandler19, KERNEL_CODE_SEGMENT, 0);

	InitIdt(&pIdtTable[INTERRUPT_NUMBER_TIMER], TimerHandler, KERNEL_CODE_SEGMENT, 0);
	InitIdt(&pIdtTable[INTERRUPT_NUMBER_KEYBOARD], KeyboardHandler, KERNEL_CODE_SEGMENT, 0);
	InitIdt(&pIdtTable[INTERRUPT_NUMBER_MOUSE], MouseHandler, KERNEL_CODE_SEGMENT, 0);

	/* 메모리 초기화 */
	InitMemory();

	DrawStringFormat(0, 60, BLACK, "Total Memory : 0x%x", GetMemorySize());

	/* 태스크 초기화 */
	InitTaskStruct();

	/* PIC 설정 */
	InitPic();

	/* 인터럽트 활성화 및 커널 데이터 초기화 */
	InitKernelData();

	AsmSti();
	SetPicInterrupt(~(PIC_KEYBOARD | PIC_SLAVE | PIC_MOUSE | PIC_TIMER));

	/* 타이머 초기화 */
	InitTimer();

	/* 키보드 초기화 */
	InitKeyboard();

	/* 마우스 초기화 */
	InitMouse();

	for (i = 0; i < 12; i++)
	{
		const TaskPriority t[] =
			{ TASK_PRIORITY_LOW, TASK_PRIORITY_NORMAL, TASK_PRIORITY_HIGH };
		void *stack = AllocateMemory(2 * 1024 * 1024);
		void testtask();
		CreateTask((DWORD)testtask, (DWORD)stack, t[i % 3]);
	}
	SetTimer(1000, 1);

	/* 큐 루프 */
	while (1)
	{
		AsmCli();

		while (!g_kData.InterruptQueue.bEmpty)
		{
			QueueData = GetQueue32(&g_kData.InterruptQueue, FALSE, NULL);

			switch (QueueData & 0xff000000)
			{
			case INTERRUPT_QUEUE_FLAG_TIMER:
				switch (QueueData & 0xffffff)
				{
				case 1:
					DrawBoxFill(500, 0, 700, 20, BACK_COLOR);
					DrawStringFormat(500, 0, BLACK, "Processor Load : %d%%",
							g_TaskStruct.ProcessorLoad);
					SetTimer(1000, 1);
					break;
				}
				break;
			case INTERRUPT_QUEUE_FLAG_KEYBOARD:
				OnKeyboardInterrupt(QueueData & 0xffffff);
				break;
			case INTERRUPT_QUEUE_FLAG_MOUSE:
				OnMouseInterrupt(QueueData & 0xffffff);
				break;
			}
		}

		AsmSti();
		TaskSchedule();
	}
}

void testtask()
{
	unsigned count = 0;
	DWORD id = GetTaskId(GetCurrentTask());
	DWORD y = id * 20 + 200;

	while (count < 0x50000)
	{
		if (count % 50 == 0)
		{
			DrawBoxFill(0, y, 200, y + 20, BACK_COLOR);
			DrawStringFormat(0, y, BLACK, "Task %d Running [0x%x]", id, count);
		}
		count++;
	}

	DrawBoxFill(0, 200, 200, 500, BACK_COLOR);
	if (id == 4)
		TerminateTask(&g_TaskStruct.tasks[2]);
	ExitTask();
}
