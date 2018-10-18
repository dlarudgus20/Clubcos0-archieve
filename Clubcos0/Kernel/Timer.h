#ifndef Timer_h_
#define Timer_h_

// PIT 타이머
#define PIT_FREQUENCY 1193180
#define TIMER_PIT_FREQUENCY (PIT_FREQUENCY / 1000)

#define PIT_CONTROL_PORT 0x43
#define PIT_COUNTER0_PORT 0x40

#define PIT_CONTROL_COUNTER0 0x00
#define PIT_CONTROL_LSBMSBRW 0x30
#define PIT_CONTROL_LATCH 0x00
#define PIT_CONTROL_MODE0 0x00
#define PIT_CONTROL_MODE2 0x04
#define PIT_CONTROL_BINARYCOUNTER 0x00
#define PIT_CONTROL_BCDCOUNTER 0x01

#define PIT_COUNTER0_ONCE \
	(PIT_CONTROL_COUNTER0 | PIT_CONTROL_LSBMSBRW | PIT_CONTROL_MODE0 | PIT_CONTROL_BINARYCOUNTER)
#define PIT_COUNTER0_PERIODIC \
	(PIT_CONTROL_COUNTER0 | PIT_CONTROL_LSBMSBRW | PIT_CONTROL_MODE2 | PIT_CONTROL_BINARYCOUNTER)

// RTC
#define RTC_CMOSADDRESS 0x70
#define RTC_CMOSDATA 0x71

#define RTC_ADDRESS_SECOND 0x00
#define RTC_ADDRESS_MINUTE 0x02
#define RTC_ADDRESS_HOUR 0x04
#define RTC_ADDRESS_DAYOFWEEK 0x06
#define RTC_ADDRESS_DAYOFMONTH 0x07
#define RTC_ADDRESS_MONTH 0x08
#define RTC_ADDRESS_YEAR 0x09

#define RTC_BCDTOBINARY(b) ((((b) >> 4) * 10) + ((b) & 0x0f))

#define MAX_TIMER 256

typedef struct tagTimer
{
	DWORD time, code;
} Timer;

typedef struct tagTimerStruct
{
	DWORD TickCountHigh, TickCountLow;

	Timer timers[MAX_TIMER];
	DWORD timeout;
	BOOL bExistTimer;
} TimerStruct;

typedef struct tagTime
{
	BYTE hour, minute, second;
} Time;

typedef struct tagDate
{
	WORD year;
	BYTE month, dayofmonth, dayofweek;
} Date;

extern TimerStruct g_Timer;

void InitTimer();
BOOL SetTimer(DWORD time, DWORD code);

void ReadRTCTime(Time *pTime);
void ReadRTCDate(Date *pDate);

const char *DayOfWeekStr(BYTE dayofweek);

/* 어셈블리 함수 */
void GetTimeStamp(DWORD *high, DWORD *low);

#endif // Timer_h_
