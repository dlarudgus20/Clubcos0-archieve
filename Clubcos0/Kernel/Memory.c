#include "Clubcos0.h"
#include "Memory.h"
#include "AssemblyFunc.h"

static int CalculateMetaBlockCount(DWORD qwDynamicRAMSize);
static int AllocationBuddyBlock(DWORD qwAlignedSize);
static DWORD GetBuddyBlockSize(DWORD qwSize);
static int GetBlockListIndexOfMatchSize(DWORD qwAlignedSize);
static int FindFreeBlockInBitmap(int iBlockListIndex);
static void SetFlagInBitmap(int iBlockListIndex, int iOffset, BYTE bFlag);
static BOOL FreeBuddyBlock(int iBlockListIndex, int iBlockOffset);
static BYTE GetFlagInBitmap(int iBlockListIndex, int iOffset);
static DWORD CheckMemorySize();

DynMemStruct g_DynMem;

void InitMemory()
{
	DWORD qwDynamicMemorySize;
	int i, j;
	BYTE* pbCurrentBitmapPosition;
	int iBlockCountOfLevel, iMetaBlockCount;

	// ���� �޸� �������� ����� �޸� ũ�⸦ �̿��Ͽ� ����� �����ϴµ�
	// �ʿ��� �޸� ũ�⸦ �ּ� ��� ������ ���
	g_DynMem.MemSize = CheckMemorySize();
	qwDynamicMemorySize = g_DynMem.MemSize - DYNAMICMEMORY_START_ADDRESS;
	iMetaBlockCount = CalculateMetaBlockCount( qwDynamicMemorySize );

	// ��ü ��� �������� ������ �ʿ��� ��Ÿ����� ������ ������ ������ ������ ���ؼ�
	// ��Ÿ ������ ����
	g_DynMem.iBlockCountOfSmallestBlock = 
		( qwDynamicMemorySize / DYNAMICMEMORY_MIN_SIZE ) - iMetaBlockCount;

	// �ִ� �� ���� ��� ����Ʈ�� �����Ǵ����� ���
	for( i = 0 ; ( g_DynMem.iBlockCountOfSmallestBlock >> i ) > 0 ; i++ )
	{
		//DO Nothing
		;
	}
	g_DynMem.iMaxLevelCount = i;
	
	// �Ҵ�� �޸𸮰� ���� ��� ����Ʈ�� �ε����� �����ϴ� ������ �ʱ�ȭ
	g_DynMem.pbAllocatedBlockListIndex = ( BYTE* ) DYNAMICMEMORY_START_ADDRESS;
	for( i = 0 ; i < g_DynMem.iBlockCountOfSmallestBlock ; i++ )
	{
		g_DynMem.pbAllocatedBlockListIndex[ i ] = 0xFF;
	}
	
	// ��Ʈ�� �ڷᱸ���� ���� ��巹�� ����
	g_DynMem.pstBitmapOfLevel = ( Bitmap* ) ( DYNAMICMEMORY_START_ADDRESS +
		( sizeof( BYTE ) * g_DynMem.iBlockCountOfSmallestBlock ) );
	// ���� ��Ʈ���� ��巹���� ����
	pbCurrentBitmapPosition = ( ( BYTE* ) g_DynMem.pstBitmapOfLevel ) + 
		( sizeof( Bitmap ) * g_DynMem.iMaxLevelCount );
	// ��� ����Ʈ ���� ������ ���鼭 ��Ʈ���� ���� 
	// �ʱ� ���´� ���� ū ��ϰ� ������ ��ϸ� �����ϹǷ� �������� ����ִ� ������ ����
	for( j = 0 ; j < g_DynMem.iMaxLevelCount ; j++ )
	{
		g_DynMem.pstBitmapOfLevel[ j ].pbBitmap = pbCurrentBitmapPosition;
		g_DynMem.pstBitmapOfLevel[ j ].qwExistBitCount = 0;
		iBlockCountOfLevel = g_DynMem.iBlockCountOfSmallestBlock >> j;

		// 8�� �̻� �������� ���� ������� ��� ������ �� �����Ƿ�, ��� ����ִ� ������ ����
		for( i = 0 ; i < iBlockCountOfLevel / 8 ; i++ )
		{
			*pbCurrentBitmapPosition = 0x00;
			pbCurrentBitmapPosition++;
		}

		// 8�� ������ �������� �ʴ� ������ ��ϵ鿡 ���� ó��
		if( ( iBlockCountOfLevel % 8 ) != 0 )
		{
			*pbCurrentBitmapPosition = 0x00;
			// ���� ����� Ȧ����� ������ �� ����� ���յǾ� ���� ������� �̵����� ����
			// ���� ������ ����� ���� ��� ����Ʈ�� �����ϴ� ������ ������� ����
			i = iBlockCountOfLevel % 8;
			if( ( i % 2 ) == 1 )
			{
				*pbCurrentBitmapPosition |= ( DYNAMICMEMORY_EXIST << ( i - 1 ) );
				g_DynMem.pstBitmapOfLevel[ j ].qwExistBitCount = 1;
			}
			pbCurrentBitmapPosition++;
		}
	}        
	
	// ��� Ǯ�� ��巹���� ���� �޸� ũ�� ����
	g_DynMem.qwStartAddress = DYNAMICMEMORY_START_ADDRESS + 
		iMetaBlockCount * DYNAMICMEMORY_MIN_SIZE;
	g_DynMem.qwEndAddress = qwDynamicMemorySize + 
		DYNAMICMEMORY_START_ADDRESS;
	g_DynMem.qwUsedSize = 0;
}

/**
 *  ���� �޸� ������ �����ϴµ� �ʿ��� ������ �����ϴ� ������ ���
 *      �ּ� ��� ������ �����ؼ� ��ȯ
 */
static int CalculateMetaBlockCount( DWORD qwDynamicRAMSize )
{
	long lBlockCountOfSmallestBlock;
	DWORD dwSizeOfAllocatedBlockListIndex;
	DWORD dwSizeOfBitmap;
	long i;
	
	// ���� ũ�Ⱑ ���� ����� ������ ����Ͽ� �̸� �������� ��Ʈ�� ������ 
	// �Ҵ�� ũ�⸦ �����ϴ� ������ ���
	lBlockCountOfSmallestBlock = qwDynamicRAMSize / DYNAMICMEMORY_MIN_SIZE;
	// �Ҵ�� ����� ���� ��� ����Ʈ�� �ε����� �����ϴµ� �ʿ��� ������ ���
	dwSizeOfAllocatedBlockListIndex = lBlockCountOfSmallestBlock * sizeof( BYTE );
	
	// ��Ʈ���� �����ϴµ� �ʿ��� ���� ���
	dwSizeOfBitmap = 0;
	for( i = 0 ; ( lBlockCountOfSmallestBlock >> i ) > 0 ; i++ )
	{
		// ��� ����Ʈ�� ��Ʈ�� �����͸� ���� ����
		dwSizeOfBitmap += sizeof( Bitmap );
		// ��� ����Ʈ�� ��Ʈ�� ũ��, ����Ʈ ������ �ø� ó��
		dwSizeOfBitmap += ( ( lBlockCountOfSmallestBlock >> i ) + 7 ) / 8;
	}
	
	// ����� �޸� ������ ũ�⸦ �ּ� ��� ũ��� �ø��ؼ� ��ȯ
	return ( dwSizeOfAllocatedBlockListIndex + dwSizeOfBitmap + 
		DYNAMICMEMORY_MIN_SIZE - 1 ) / DYNAMICMEMORY_MIN_SIZE;
}

/**
 *  �޸𸮸� �Ҵ�
 */
void* AllocateMemory( DWORD qwSize )
{
	DWORD qwAlignedSize;
	DWORD qwRelativeAddress;
	long lOffset;
	int iSizeArrayOffset;
	int iIndexOfBlockList;

	// �޸� ũ�⸦ ���� ����� ũ��� ����
	qwAlignedSize = GetBuddyBlockSize( qwSize );
	if( qwAlignedSize == 0 )
	{
		return NULL;
	}
	
	// ���� ���� ������ ������� ������ ����
	if( g_DynMem.qwStartAddress + g_DynMem.qwUsedSize +
			qwAlignedSize > g_DynMem.qwEndAddress )
	{
		return NULL;
	}

	// ���� ��� �Ҵ��ϰ� �Ҵ�� ����� ���� ��� ����Ʈ�� �ε����� ��ȯ
	lOffset = AllocationBuddyBlock( qwAlignedSize );
	if( lOffset == -1 )
	{
		return NULL;
	}
	
	iIndexOfBlockList = GetBlockListIndexOfMatchSize( qwAlignedSize );
	
	// ��� ũ�⸦ �����ϴ� ������ ������ �Ҵ�� ���� ����� ���� �� ����Ʈ�� 
	// �ε����� ����
	// �޸𸮸� ������ �� ��� ����Ʈ�� �ε����� ���
	qwRelativeAddress = qwAlignedSize * lOffset;
	iSizeArrayOffset = qwRelativeAddress / DYNAMICMEMORY_MIN_SIZE;
	g_DynMem.pbAllocatedBlockListIndex[ iSizeArrayOffset ] = 
		( BYTE ) iIndexOfBlockList;
	g_DynMem.qwUsedSize += qwAlignedSize;
	
	return ( void* ) ( qwRelativeAddress + g_DynMem.qwStartAddress );
}

/**
 *  ���� ����� ���� ����� ũ��� ���ĵ� ũ�⸦ ��ȯ
 */
static DWORD GetBuddyBlockSize( DWORD qwSize )
{
	long i;

	for( i = 0 ; i < g_DynMem.iMaxLevelCount ; i++ )
	{
		if( qwSize <= ( DYNAMICMEMORY_MIN_SIZE << i ) )
		{
			return ( DYNAMICMEMORY_MIN_SIZE << i );
		}
	}
	return 0;
}

/**
 *  ���� ��� �˰������� �޸� ����� �Ҵ�
 *      �޸� ũ��� ���� ����� ũ��� ��û�ؾ� ��
 */
static int AllocationBuddyBlock( DWORD qwAlignedSize )
{
	int iBlockListIndex, iFreeOffset;
	int i;
	DWORD eflag;

	// ��� ũ�⸦ �����ϴ� ��� ����Ʈ�� �ε����� �˻�
	iBlockListIndex = GetBlockListIndexOfMatchSize( qwAlignedSize );
	if( iBlockListIndex == -1 )
	{
		return -1;
	}
	
	// ����ȭ ó��
	eflag = GetEFlag();
	AsmCli();
	
	// �����ϴ� ��� ����Ʈ���� �ֻ��� ��� ����Ʈ���� �˻��Ͽ� ����� ����
	for( i = iBlockListIndex ; i< g_DynMem.iMaxLevelCount ; i++ )
	{
		// ��� ����Ʈ�� ��Ʈ���� Ȯ���Ͽ� ����� �����ϴ��� Ȯ��
		iFreeOffset = FindFreeBlockInBitmap( i );
		if( iFreeOffset != -1 )
		{
			break;
		}
	}
	
	// ������ ��� ����Ʈ���� �˻��ߴµ��� ������ ����
	if( iFreeOffset == -1 )
	{
	  //  UnlockForSystemData( bPreviousInterruptFlag );
		return -1;
	}

	// ����� ã������ �� ������ ǥ��
	SetFlagInBitmap( i, iFreeOffset, DYNAMICMEMORY_EMPTY );

	// ���� ��Ͽ��� ����� ã�Ҵٸ� ���� ����� ����
	if( i > iBlockListIndex )
	{
		// �˻��� ��� ����Ʈ���� �˻��� ������ ��� ����Ʈ���� �������鼭 ���� �����
		// �� ������ ǥ���ϰ� ������ ����� �����ϴ� ������ ǥ����
		for( i = i - 1 ; i >= iBlockListIndex ; i-- )
		{
			// ���� ����� �� ������ ǥ��
			SetFlagInBitmap( i, iFreeOffset * 2, DYNAMICMEMORY_EMPTY );
			// ������ ����� �����ϴ� ������ ǥ��
			SetFlagInBitmap( i, iFreeOffset * 2 + 1, DYNAMICMEMORY_EXIST ); 
			// ���� ����� �ٽ� ����
			iFreeOffset = iFreeOffset * 2;
		}
	}    
   SetEFlag(eflag);
	
	return iFreeOffset;
}

/**
 *  ���޵� ũ��� ���� ������ ��� ����Ʈ�� �ε����� ��ȯ
 */
static int GetBlockListIndexOfMatchSize( DWORD qwAlignedSize )
{
	int i;

	for( i = 0 ; i < g_DynMem.iMaxLevelCount ; i++ )
	{
		if( qwAlignedSize <= ( DYNAMICMEMORY_MIN_SIZE << i ) )
		{
			return i;
		}
	}
	return -1;
}

/**
 *  ��� ����Ʈ�� ��Ʈ�ʸ� �˻��� ��, ����� �����ϸ� ����� �������� ��ȯ
 */
static int FindFreeBlockInBitmap( int iBlockListIndex )
{
	int i, iMaxCount;
	BYTE* pbBitmap;
	DWORD* pqwBitmap;

	// ��Ʈ�ʿ� �����Ͱ� �������� �ʴ´ٸ� ����
	if( g_DynMem.pstBitmapOfLevel[ iBlockListIndex ].qwExistBitCount == 0 )
	{
		return -1;
	}
	
	// ��� ����Ʈ�� �����ϴ� �� ����� ���� ���� ��, �� ������ŭ ��Ʈ���� �˻�
	iMaxCount = g_DynMem.iBlockCountOfSmallestBlock >> iBlockListIndex;
	pbBitmap = g_DynMem.pstBitmapOfLevel[ iBlockListIndex ].pbBitmap;
	for( i = 0 ; i < iMaxCount ; )
	{
		// DWORD�� 8 * 8��Ʈ => 64��Ʈ�̹Ƿ�, 64��Ʈ�� �Ѳ����� �˻��ؼ� 1�� ��Ʈ��
		// �ִ� �� Ȯ����
		if( ( ( iMaxCount - i ) / 64 ) > 0 )
		{
			pqwBitmap = ( DWORD* ) &( pbBitmap[ i / 8 ] );
			// ���� 8����Ʈ�� ��� 0�̸� 8����Ʈ ��� ����
			if( *pqwBitmap == 0 )
			{
				i += 32;
				continue;
			}
		}                
		
		if( ( pbBitmap[ i / 8 ] & ( DYNAMICMEMORY_EXIST << ( i % 8 ) ) ) != 0 )
		{
			return i;
		}
		i++;
	}
	return -1;
}

/**
 *  ��Ʈ�ʿ� �÷��׸� ����
 */
static void SetFlagInBitmap( int iBlockListIndex, int iOffset, BYTE bFlag )
{
	BYTE* pbBitmap;

	pbBitmap = g_DynMem.pstBitmapOfLevel[ iBlockListIndex ].pbBitmap;
	if( bFlag == DYNAMICMEMORY_EXIST )
	{
		// �ش� ��ġ�� �����Ͱ� ��� �ִٸ� ���� ����
		if( ( pbBitmap[ iOffset / 8 ] & ( 0x01 << ( iOffset % 8 ) ) ) == 0 )
		{
			g_DynMem.pstBitmapOfLevel[ iBlockListIndex ].qwExistBitCount++;
		}
		pbBitmap[ iOffset / 8 ] |= ( 0x01 << ( iOffset % 8 ) );
	}
	else 
	{
		// �ش� ��ġ�� �����Ͱ� �����Ѵٸ� ���� ����
		if( ( pbBitmap[ iOffset / 8 ] & ( 0x01 << ( iOffset % 8 ) ) ) != 0 )
		{
			g_DynMem.pstBitmapOfLevel[ iBlockListIndex ].qwExistBitCount--;
		}
		pbBitmap[ iOffset / 8 ] &= ~( 0x01 << ( iOffset % 8 ) );
	}
}

/**
 *  �Ҵ� ���� �޸𸮸� ����
 */
BOOL FreeMemory( void* pvAddress )
{
	DWORD qwRelativeAddress;
	int iSizeArrayOffset;
	DWORD qwBlockSize;
	int iBlockListIndex;
	int iBitmapOffset;

	if( pvAddress == NULL )
	{
		return FALSE;
	}

	// �Ѱ� ���� ��巹���� ��� Ǯ�� �������� �ϴ� ��巹���� ��ȯ�Ͽ� �Ҵ��ߴ�
	// ����� ũ�⸦ �˻�
	qwRelativeAddress = ( ( DWORD ) pvAddress ) - g_DynMem.qwStartAddress;
	iSizeArrayOffset = qwRelativeAddress / DYNAMICMEMORY_MIN_SIZE;

	// �Ҵ�Ǿ����� ������ ��ȯ �� ��
	if( g_DynMem.pbAllocatedBlockListIndex[ iSizeArrayOffset ] == 0xFF )
	{
		return FALSE;
	}

	// �Ҵ�� ����� ���� ��� ����Ʈ�� �ε����� ����� ���� �ʱ�ȭ�ϰ�, �Ҵ�� 
	// ����� ���Ե� ��� ����Ʈ�� �˻�
	iBlockListIndex = ( int ) g_DynMem.pbAllocatedBlockListIndex[ iSizeArrayOffset ];
	g_DynMem.pbAllocatedBlockListIndex[ iSizeArrayOffset ] = 0xFF;
	// ���� ����� �ּ� ũ�⸦ ��� ����Ʈ �ε����� ����Ʈ�Ͽ� �Ҵ�� ����� ũ�� ���
	qwBlockSize = DYNAMICMEMORY_MIN_SIZE << iBlockListIndex;

	// ��� ����Ʈ ���� ��� �������� ���ؼ� ��� ����
	iBitmapOffset = qwRelativeAddress / qwBlockSize;
	if( FreeBuddyBlock( iBlockListIndex, iBitmapOffset ) == TRUE )
	{
		g_DynMem.qwUsedSize -= qwBlockSize;
		return TRUE;
	}
	
	return FALSE;
}

/**
 *  ��� ����Ʈ�� ���� ����� ����
 */
static BOOL FreeBuddyBlock( int iBlockListIndex, int iBlockOffset )
{
	int iBuddyBlockOffset;
	int i;
	BOOL bFlag;
	DWORD eflag;

	// ����ȭ ó��
	eflag = GetEFlag();
	AsmCli();
	
	// ��� ����Ʈ�� ������ ������ ����� �˻��Ͽ� ������ �� ���� ������ �ݺ�
	for( i = iBlockListIndex ; i < g_DynMem.iMaxLevelCount ; i++ )
	{
		// ���� ����� �����ϴ� ���·� ����
		SetFlagInBitmap( i, iBlockOffset, DYNAMICMEMORY_EXIST );
		
		// ����� �������� ¦��(����)�̸� Ȧ��(������)�� �˻��ϰ�, Ȧ���̸� ¦����
		// ��Ʈ���� �˻��Ͽ� ������ ����� �����Ѵٸ� ����
		if( ( iBlockOffset % 2 ) == 0 )
		{
			iBuddyBlockOffset = iBlockOffset + 1;
		}
		else
		{
			iBuddyBlockOffset = iBlockOffset - 1;
		}
		bFlag = GetFlagInBitmap( i, iBuddyBlockOffset );

		// ����� ��������� ����
		if( bFlag == DYNAMICMEMORY_EMPTY )
		{
			break;
		}
		
		// ������� �Դٸ� ������ ����� �����ϹǷ�, ����� ����
		// ����� ��� �� ������ ����� ���� ������� �̵�
		SetFlagInBitmap( i, iBuddyBlockOffset, DYNAMICMEMORY_EMPTY );
		SetFlagInBitmap( i, iBlockOffset, DYNAMICMEMORY_EMPTY );
		
		// ���� ��� ����Ʈ�� ��� ���������� �����ϰ�, ���� ������ ���� ��Ͽ���
		// �ٽ� �ݺ�
		iBlockOffset = iBlockOffset/ 2;
	}
	
	SetEFlag(eflag);
	return TRUE;
}

/**
 *  ��� ����Ʈ�� �ش� ��ġ�� ��Ʈ���� ��ȯ
*/
static BYTE GetFlagInBitmap( int iBlockListIndex, int iOffset )
{
	BYTE* pbBitmap;
	
	pbBitmap = g_DynMem.pstBitmapOfLevel[ iBlockListIndex ].pbBitmap;
	if( ( pbBitmap[ iOffset / 8 ] & ( 0x01 << ( iOffset % 8 ) ) ) != 0x00 )
	{
		return DYNAMICMEMORY_EXIST;
	}
	
	return DYNAMICMEMORY_EMPTY;
}

DWORD GetMemorySize()
{
	return g_DynMem.MemSize;
}

static BOOL CheckMemorySizeSub(volatile DWORD *ptr)
{
	volatile DWORD old = *ptr;
	BOOL ret = FALSE;
	*ptr = 0x1234abcd;
	*ptr ^= 0xffffffff;
	if (*ptr == ~0x1234abcd)
	{
		*ptr ^= 0xffffffff;
		if (*ptr == 0x1234abcd)
		{
			ret = TRUE;
		}
	}
	*ptr = old;
	return ret;
}
static DWORD CheckMemorySize()
{
	DWORD eflag, cr0;
	BOOL b486 = FALSE;

	volatile DWORD ptr;

	eflag = GetEFlag();
	SetEFlag(eflag | EFLAG_AC);
	if ((GetEFlag() & EFLAG_AC) != 0)
		b486 = TRUE;
	SetEFlag(eflag);

	if (b486)
	{
		cr0 = GetCr0();
		SetCr0(cr0 | CR0_CACHE_DISABLE);
	}

	for (ptr = DYNAMICMEMORY_START_ADDRESS; ptr < 0x30000000; ptr += 0x100000)
	{
		if (!CheckMemorySizeSub((volatile DWORD *)ptr))
			break;
	}
	if (!CheckMemorySizeSub((volatile DWORD *)(ptr - 4)))
	{
		ptr -= 0x100000;
	}

	if (b486)
	{
		SetCr0(cr0);
	}

	return ptr;
}
