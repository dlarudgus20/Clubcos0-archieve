#ifndef Memory_h_
#define Memory_h_

#define DYNAMICMEMORY_START_ADDRESS 0x700000

// ���� ����� �ּ� ũ��, 1KB
#define DYNAMICMEMORY_MIN_SIZE          ( 1 * 1024 )

// ��Ʈ���� �÷���
#define DYNAMICMEMORY_EXIST             0x01
#define DYNAMICMEMORY_EMPTY             0x00

typedef struct tagBitmap
{
	BYTE* pbBitmap;
	DWORD qwExistBitCount;
} Bitmap;

typedef struct tagDynMemStruct
{
	DWORD MemSize;

	// ��� ����Ʈ�� �� ������ ���� ũ�Ⱑ ���� ���� ����� ����, �׸��� �Ҵ�� �޸� ũ��
	int iMaxLevelCount;
	int iBlockCountOfSmallestBlock;
	DWORD qwUsedSize;

	// ��� Ǯ�� ���� ��巹���� ������ ��巹��
	DWORD qwStartAddress;
	DWORD qwEndAddress;

	// �Ҵ�� �޸𸮰� ���� ��� ����Ʈ�� �ε����� �����ϴ� ������ ��Ʈ�� �ڷᱸ���� 
	// ��巹��
	BYTE* pbAllocatedBlockListIndex;
	Bitmap* pstBitmapOfLevel;
} DynMemStruct;

extern DynMemStruct g_DynMem;

void InitMemory();
void* AllocateMemory(DWORD qwSize);
BOOL FreeMemory(void* pvAddress);

DWORD GetMemorySize();

#endif // Memory_h_
