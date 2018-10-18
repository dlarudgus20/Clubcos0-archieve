#include "Clubcos0.h"
#include "Interrupt.h"
#include "IDT.h"
#include "PIC.h"
#include "AssemblyFunc.h"

void InitPic()
{
	DWORD eflag;

	eflag = GetEFlag();
	AsmCli();

	OutByte(MASTER_PIC_PORT1, 0x11); /* ���� Ʈ����, ĳ�����̵� ���, ICW4 ��� */

	OutByte(MASTER_PIC_PORT2, PIC_INTERRUPT_NUMBER); /* PIC�� ���ͷ�Ʈ ��ȣ ���� */

	OutByte(MASTER_PIC_PORT2, 0x04); /* �����̺� PIC�� 2��° ��(IRQ 1)�� ����� */

	OutByte(MASTER_PIC_PORT2, 0x01); /* 8086/88 ��� */

	OutByte(SLAVE_PIC_PORT1, 0x11);
	OutByte(SLAVE_PIC_PORT2, PIC_INTERRUPT_NUMBER + 8);
	OutByte(SLAVE_PIC_PORT2, 2);
	OutByte(SLAVE_PIC_PORT2, 0x01); /* 8086/88 ��� */

	SetPicInterrupt(~PIC_SLAVE);

	SetEFlag(eflag);
}

void SetPicInterrupt(WORD IrqField)
{
	OutByte(MASTER_PIC_PORT2, (BYTE)IrqField);
	OutByte(SLAVE_PIC_PORT2, (BYTE)(IrqField >> 8));
}

void SendEOI(BYTE irq)
{
	if (irq < 8)
	{
		OutByte(MASTER_PIC_PORT1, irq | 0x60);
	}
	else
	{
		OutByte(MASTER_PIC_PORT1, PIC_IRQ_SLAVE | 0x60);
		OutByte(SLAVE_PIC_PORT1, (irq - 8) | 0x60);
	}
}
