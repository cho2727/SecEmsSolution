#include <windows.h>
#include <direct.h>
#include <io.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <stdio.h>

#include "shm/SecShardMem.h"
#include "sec/SecCommTypes.h"
#include "sec/SecErrorCode.h"

#define			SHM_PDB_KIND_ENV_NAME		"SHM_PDB_KIND"


static const sec_shm_id_st sec_shm_id_tbl[] =
{
	{ SEC_PDB_UNKNOWN,			0,									SEC_MAX_UNKNOWN,					"pdb_Unknown",					"Unknown",				"UNKNOWN"},
	{ SEC_PDB_PROCINFO,			sizeof(int),						SEC_MAX_PDB_PROCINFO,				"pdb_procinfo",					"procinfo.pdb",			"PROCINFO"},
	{ SEC_PDB_NODEINFO,			sizeof(int),						SEC_MAX_PDB_NODEINFO,				"pdb_nodeinfo",					"nodeinfo.pdb",			"NODEINFO"},

	{ SEC_PDB_RTU_DEVICE,		sizeof(ShardRtuDevice),				SEC_MAX_PDB_RTU_INFO,				"pdb_rtudvice",					"rtudvice.pdb",			"RTUDEVICE"},
	{ SEC_PDB_RTU_BI,			sizeof(ShardBinaryInput),			SEC_MAX_PDB_RTU_BI,					"pdb_rtu_bi",					"rtu_bi.pdb",			"RTUBI"},
	{ SEC_PDB_RTU_BO,			sizeof(ShardBinaryOutput),			SEC_MAX_PDB_RTU_BO,					"pdb_rtu_bo",					"rtu_bo.pdb",			"RTUBO"},
	{ SEC_PDB_RTU_AI,			sizeof(ShardAnalogInput),			SEC_MAX_PDB_RTU_AI,					"pdb_rtu_ai",					"rtu_ai.pdb",			"RTUAI"},
	{ SEC_PDB_RTU_AO,			sizeof(ShardAnalogOutput),			SEC_MAX_PDB_RTU_AO,					"pdb_rtu_ao",					"rtu_ao.pdb",			"RTUAO"},
	{ SEC_PDB_RTU_CNT,			sizeof(ShardCounter),				SEC_MAX_PDB_RTU_CNT,				"pdb_rtu_cnt",					"rtu_cnt.pdb",			"RTUCNT"},
	/////////////////////////////////////////////////////////
	{ SEC_PDB_RESERVED,			0,									SEC_MAX_RESERVED,					"pdb_Reserved",					"Reserved",				"RESERVED"}
};


SecShardMem::SecShardMem(void)
	: shmKind_(0), shmHandle_(0), shmArea_(NULL), mutexHandle_(0)
{
}


SecShardMem::~SecShardMem(void)
{
}

SecShardMem* SecShardMem::GetInstance()
{
	static SecShardMem _instance;
	return &_instance;
}

int		SecShardMem::SM_Init()
{
	int		ret;
	char	*ptr;

	shmKind_ = SEC_PDB_KIND_SHM;


	switch( shmKind_ )
	{
	case SEC_PDB_KIND_SHM	:
		ret = SM_Initialize();
		break;
	}

	SEM_Initialize();

	return( ret );
}

int		SecShardMem::SM_Uninit()
{
	int		ret;
	switch( shmKind_ )
	{
	case SEC_PDB_KIND_SHM	:
		SM_DestroyMemory();
		break;
	}

	ret = SEM_DestroySema();

	return( ret );
}

int		SecShardMem::SM_Initialize( void )		// 공유 메모리 생성 및 셋팅
{
	if( shmHandle_ != INVALID_HANDLE_VALUE && shmArea_ != NULL )
	{
		fprintf( stdout, "shard memory already init\n" );
		return SEC_OK;
	}

	shmHandle_ = OpenFileMappingA( FILE_MAP_ALL_ACCESS, FALSE, SEC_FILEMAP_NAME ); // 공유 메모리 오픈
	if(shmHandle_ == NULL)
	{
		shmHandle_ = CreateFileMappingA( INVALID_HANDLE_VALUE,
			NULL,
			PAGE_READWRITE,
			0,    
			sizeof(pdb_area_st),    
			SEC_FILEMAP_NAME );

		if( shmHandle_ == NULL )
		{
			fprintf( stdout, "shard memory createfilemapping fail\n" );
			return SEC_ERR;
		}
	}

	shmArea_ = (pdb_area_st *) MapViewOfFile( shmHandle_, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(pdb_area_st) );
	if( shmArea_ == NULL )
	{
		fprintf( stdout, "shard memory mapviewoffile fail\n" );
		return SEC_ERR;
	}
	
	return SEC_OK;
}


void	SecShardMem::SM_DestroyMemory( void )
{
	if( shmArea_ != NULL )
	{
		UnmapViewOfFile( shmArea_ );
		shmArea_ = NULL;
	}

	if( shmHandle_ != NULL)
	{
		::CloseHandle( shmHandle_ );
		shmHandle_ = NULL;
	}
}


int			SecShardMem::SEM_Initialize( void )
{
	mutexHandle_ = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, SEC_LOGMSG_MUTEX_NAME);
	if(mutexHandle_ == NULL)
	{
		mutexHandle_ = CreateMutexA(NULL, FALSE, SEC_LOGMSG_MUTEX_NAME);

		if(mutexHandle_ == NULL)
			return ( SEC_ERR );
	}

	return ( SEC_OK );
}

int	SecShardMem::SEM_DestroySema( void )
{
	if(!CloseHandle(mutexHandle_ )) // 뮤텍스 해제
		return ( SEC_ERR );

	return ( SEC_OK ); 
}


///////////////////////////////////////////
void*	SecShardMem::SM_GetPointer( int pdbNo )
{
	void	*ptr = NULL;

	if( pdbNo <= SEC_PDB_UNKNOWN || pdbNo >= SEC_PDB_RESERVED )
		return ptr;

	switch( pdbNo )
	{
	case SEC_PDB_PROCINFO			:	ptr = (void *)shmArea_->pdb_procinfo;				break;
	case SEC_PDB_NODEINFO			:	ptr = (void *)shmArea_->pdb_nodeinfo;				break;
	}

	return ptr;
}

int		SecShardMem::SM_GetMaxCount( int pdbNo )
{
	return 0;
}

int		SecShardMem::SM_GetItemSize( int pdbNo )
{
	return 0;
}

int		SecShardMem::SM_GetItemCount( int pdbNo )
{
	return 0;
}