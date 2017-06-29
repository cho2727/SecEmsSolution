#ifndef _SEC_SHARD_MEM_INTO_H_
#define _SEC_SHARD_MEM_INTO_H_

#include "shm/SecShardStruct.h"
/*----------------------------------------------------------------------------*/
/* Definition                                                                 */
/*----------------------------------------------------------------------------*/
#define	SEC_FILEMAP_NAME			"SEC_FILEMAP"
#define	SEC_DEF_PDB_DIRECTORY		".\\Data\\pdb"

#define	SEC_PDB_KIND_DEFAULT		SEC_PDB_KIND_SHM
#define	SEC_PDB_KIND_SHM			0x0001
#define	SEC_PDB_KIND_FILE			0x0002


#define	MAX_NOOF_PROC				100		// 최대 Process 갯수

#define	SEC_MAX_UNKNOWN				    0
#define	SEC_MAX_PDB_PROCINFO		   MAX_NOOF_PROC
#define SEC_MAX_PDB_NODEINFO			16
#define	SEC_MAX_RESERVED				0
#define SEC_MAX_PDB_RTU_INFO			100
#define SEC_MAX_PDB_RTU_BI				SEC_MAX_PDB_RTU_INFO * 100
#define SEC_MAX_PDB_RTU_BO				SEC_MAX_PDB_RTU_INFO * 100
#define SEC_MAX_PDB_RTU_AI				SEC_MAX_PDB_RTU_INFO * 100
#define SEC_MAX_PDB_RTU_AO				SEC_MAX_PDB_RTU_INFO * 100
#define SEC_MAX_PDB_RTU_CNT				SEC_MAX_PDB_RTU_INFO * 100


#define SEC_LOGMSG_MUTEX_NAME			"SHARE_LOGMSG_MUTEX"


typedef enum _secPdbNo_e
{
	SEC_PDB_UNKNOWN	= 0,
	SEC_PDB_PROCINFO,	// 프로세스 관련.
	SEC_PDB_NODEINFO,	// 프로세스 관련.
	SEC_PDB_RTU_DEVICE,
	SEC_PDB_RTU_BI,
	SEC_PDB_RTU_BO,
	SEC_PDB_RTU_AI,
	SEC_PDB_RTU_AO,
	SEC_PDB_RTU_CNT,
	/////////////////////////////////////
	SEC_PDB_RESERVED // MAX PDB ID
} secPdbNo_e;

// 전체 공유 메모리 크기
typedef	struct	_pdb_area_st	{
	int							pdb_procinfo			[SEC_MAX_PDB_PROCINFO]; // 프로세스 상태 정보
	int							pdb_nodeinfo			[SEC_MAX_PDB_NODEINFO];
	ShardRtuDevice				pdb_rtu_device			[SEC_MAX_PDB_RTU_INFO];
	ShardBinaryInput			pdb_rtu_bi				[SEC_MAX_PDB_RTU_BI];
	ShardBinaryOutput			pdb_rtu_bo				[SEC_MAX_PDB_RTU_BO];
	ShardAnalogInput			pdb_rtu_ai				[SEC_MAX_PDB_RTU_AI];
	ShardAnalogOutput			pdb_rtu_ao				[SEC_MAX_PDB_RTU_AO];
	ShardCounter				pdb_rtu_cnt				[SEC_MAX_PDB_RTU_CNT];
} pdb_area_st;



#define MAX_SHM_DESC_STR_SZ			64
#define MAX_SHM_NAME_STR_SZ			32
#define MAX_SHM_FNAME_STR_SZ		64
typedef struct _sec_shm_id_st
{
	int			iShmID;
	int			iShmSize;
	int			iShmCount;
	char		szShmName[MAX_SHM_NAME_STR_SZ]; // 공유 메모리 이름
	char		szShmFileName[MAX_SHM_FNAME_STR_SZ]; // 공유 메모리 파일(PDB)
	char		szShmDesc[MAX_SHM_DESC_STR_SZ]; // 설명.
}sec_shm_id_st;





















#endif