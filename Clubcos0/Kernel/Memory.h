#ifndef Memory_h_
#define Memory_h_

#define DYNAMICMEMORY_START_ADDRESS 0x700000

// 버디 블록의 최소 크기, 1KB
#define DYNAMICMEMORY_MIN_SIZE          ( 1 * 1024 )

// 비트맵의 플래그
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

	// 블록 리스트의 총 개수와 가장 크기가 가장 작은 블록의 개수, 그리고 할당된 메모리 크기
	int iMaxLevelCount;
	int iBlockCountOfSmallestBlock;
	DWORD qwUsedSize;

	// 블록 풀의 시작 어드레스와 마지막 어드레스
	DWORD qwStartAddress;
	DWORD qwEndAddress;

	// 할당된 메모리가 속한 블록 리스트의 인덱스를 저장하는 영역과 비트맵 자료구조의 
	// 어드레스
	BYTE* pbAllocatedBlockListIndex;
	Bitmap* pstBitmapOfLevel;
} DynMemStruct;

extern DynMemStruct g_DynMem;

void InitMemory();
void* AllocateMemory(DWORD qwSize);
BOOL FreeMemory(void* pvAddress);

DWORD GetMemorySize();

#endif // Memory_h_
