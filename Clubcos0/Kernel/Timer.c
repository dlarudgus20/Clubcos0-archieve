#include "Clubcos0.h"
#include "Timer.h"
#include "AssemblyFunc.h"
#include "KernelData.h"
#include "Queue.h"
#include "Interrupt.h"
#include "PIC.h"
#include "Graphic.h"
#include "Task.h"

TimerStruct g_Timer;

void InitTimer()
{
	memset(g_Timer.timers, 0, sizeof(g_Timer.timers));
	g_Timer.bExistTimer = FALSE;
	g_Timer.timeout = 0xffffffff;

	g_Timer.TickCountHigh = g_Timer.TickCountLow = 0;

	OutByte(PIT_CONTROL_PORT, PIT_COUNTER0_PERIODIC);

	OutByte(PIT_COUNTER0_PORT, (BYTE)TIMER_PIT_FREQUENCY);
	OutByte(PIT_COUNTER0_PORT, (BYTE)(TIMER_PIT_FREQUENCY >> 8));
}

BOOL SetTimer(DWORD time, DWORD code)
{
	Timer *ptr;
	BOOL bRet = FALSE;
	DWORD eflag;

	eflag = GetEFlag();
	AsmCli();

	time += g_Timer.TickCountLow;

	for (ptr = g_Timer.timers; ptr < g_Timer.timers + sizeof(g_Timer.timers) / sizeof(Timer); ptr++)
	{
		if (ptr->time == 0)
		{
			ptr->time = time;
			ptr->code = code;
			if (g_Timer.timeout > time)
				g_Timer.timeout = time;

			g_Timer.bExistTimer = TRUE;
			bRet = TRUE;
			break;
		}
	}

	SetEFlag(eflag);
	return bRet;
}

void _TimerHandler(DWORD *esp)
{
	Timer *ptr;

	SendEOI(PIC_IRQ_TIMER);

	g_Timer.TickCountLow++;
	if (g_Timer.TickCountLow == 0)
		g_Timer.TickCountHigh++;

	/*if (g_Timer.bExistTimer && g_Timer.timeout == g_Timer.TickCountLow)
	{
		for (ptr = g_Timer.timers; ptr < g_Timer.timers + sizeof(g_Timer.timers) / sizeof(Timer); ptr++)
		{
			if (ptr->time == g_Timer.TickCountLow)
			{
				PutQueue32(&g_kData.InterruptQueue, INTERRUPT_QUEUE_FLAG_TIMER | ptr->code);
				ptr->time = 0;
				g_Timer.timeout = 0xffffffff;
			}
			else
			{
				if (g_Timer.timeout > ptr->time)
					g_Timer.timeout = ptr->time;
			}
		}

		if (g_Timer.timeout == 0xffffffff)
		{
			g_Timer.bExistTimer = FALSE;
		}
	}*/

	if (g_Timer.TickCountLow % TASK_QUANTUM)
		TaskSchedule();
}

void ReadRTCTime(Time *pTime)
{
	OutByte(RTC_CMOSADDRESS, RTC_ADDRESS_HOUR);
	pTime->hour = InByte(RTC_CMOSDATA);
	pTime->hour = RTC_BCDTOBINARY(pTime->hour);

	OutByte(RTC_CMOSADDRESS, RTC_ADDRESS_MINUTE);
	pTime->minute = InByte(RTC_CMOSDATA);
	pTime->minute = RTC_BCDTOBINARY(pTime->minute);

	OutByte(RTC_CMOSADDRESS, RTC_ADDRESS_SECOND);
	pTime->second = InByte(RTC_CMOSDATA);
	pTime->second = RTC_BCDTOBINARY(pTime->second);
}

void ReadRTCDate(Date *pDate)
{
	BYTE b;

	OutByte(RTC_CMOSADDRESS, RTC_ADDRESS_YEAR);
	b = InByte(RTC_CMOSDATA);
	pDate->year = RTC_BCDTOBINARY(b) + 2000;

	OutByte(RTC_CMOSADDRESS, RTC_ADDRESS_MONTH);
	pDate->month = InByte(RTC_CMOSDATA);
	pDate->month = RTC_BCDTOBINARY(pDate->month);

	OutByte(RTC_CMOSADDRESS, RTC_ADDRESS_DAYOFMONTH);
	pDate->dayofmonth = InByte(RTC_CMOSDATA);
	pDate->dayofmonth = RTC_BCDTOBINARY(pDate->dayofmonth);

	OutByte(RTC_CMOSADDRESS, RTC_ADDRESS_DAYOFWEEK);
	pDate->dayofweek = InByte(RTC_CMOSDATA);
	pDate->dayofweek = RTC_BCDTOBINARY(pDate->dayofweek);
}

const char *DayOfWeekStr(BYTE dayofweek)
{
	static char *arstr[] = {
		"Error",
		"Sunday",
		"Monday",
		"Tuesday",
		"Wednesday",
		"Thursday",
		"Friday",
		"Saturday"
	};
	return arstr[(dayofweek < 8) ? dayofweek : 0];
}
