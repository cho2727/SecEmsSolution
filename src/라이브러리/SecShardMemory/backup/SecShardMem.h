#pragma once
#include "SecShardMemInfo.h"

class SecShardMem
{
public:
	SecShardMem(void);
	~SecShardMem(void);

	int		SM_Init();
	int		SM_Uninit();


private:
	int		SM_Initialize( void );
	void	SM_DestroyMemory( void );

	int		SEM_Initialize( void );
	int		SEM_DestroySema( void );

	void*	SM_GetPointer( int pdbNo );

private:
	int				shmKind_;
	HANDLE			shmHandle_;
	pdb_area_st		*shmArea_;
	HANDLE			mutexHandle_;

};

