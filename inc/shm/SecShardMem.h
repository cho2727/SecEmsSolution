#pragma once
#include "shm/SecShardMemInfo.h"

#ifndef WELLS_Export
#ifdef WELLS_DLL_EXPORTS
#define WELLS_Export __declspec(dllexport)
#else
#define WELLS_Export __declspec(dllimport)
#endif
#endif

class WELLS_Export SecShardMem
{
private:
	SecShardMem(void);
public:
	~SecShardMem(void);

	static SecShardMem* GetInstance();

	int		SM_Init();
	int		SM_Uninit();

	void*	SM_GetPointer( int pdbNo );

	int		SM_GetMaxCount( int pdbNo );
	int		SM_GetItemSize( int pdbNo );
	int		SM_GetItemCount( int pdbNo );

private:
	int		SM_Initialize( void );
	void	SM_DestroyMemory( void );

	int		SEM_Initialize( void );
	int		SEM_DestroySema( void );

private:
	int				shmKind_;
	HANDLE			shmHandle_;
	pdb_area_st		*shmArea_;
	HANDLE			mutexHandle_;

};

