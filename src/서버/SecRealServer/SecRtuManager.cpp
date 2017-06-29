#include "StdAfx.h"
#include "SecRtuManager.h"
#include "SecRtuDevice.h"
#include "SecRtuMap.h"
#include "oledb/CubeDataResource.h"
#include "oledb/CubeDynamicCommand.h"


const wchar_t* rtu_dev_query = _T("select RUID, Name, RTUMapId, FEP_PROG_ID, Datalink from RTUDevice WHERE FEP_PROG_ID is not null");

SecRtuManager::SecRtuManager(void)
{
}


SecRtuManager::~SecRtuManager(void)
{
	DelRtuDevice();

	DelRtuMap();
}

SecRtuManager* SecRtuManager::GetInstance()
{
	static SecRtuManager _instance;
	return &_instance;
}

int					SecRtuManager::RtuLoad()
{
	int iRet = 0;
	iRet = RtuMapLoad();

	iRet = RtuDeviceLoad();

	return iRet;
}

int					SecRtuManager::RtuMapLoad()
{

}

int					SecRtuManager::RtuDeviceLoad()
{
	int iRet = 0;

}

int					SecRtuManager::AddRtuMap( int map_id, int type_id, char* map_name, char* type_name, char* kind_name, int bi_cnt, int bo_cnt, int ai_cnt, int ao_cnt, int ci_cnt )
{
	if( map_id <= 0 )
		return SEC_PARAM_ERR;

	SecRtuMap* prtu_map = GetRtuMap( map_id );
	if( prtu_map != NULL )
		return SEC_DATA_EXIST;

	prtu_map = new SecRtuMap( map_id, type_id, map_name, type_name, kind_name, bi_cnt, bo_cnt, ai_cnt, ao_cnt, ci_cnt );

	rtu_point_map_.insert(pair<int, SecRtuMap*>(map_id, prtu_map));

	return SEC_OK;
}

void				SecRtuManager::DelRtuMap( )
{
	RTU_POINT_MAP::iterator it = rtu_point_map_.begin();
	while( it != rtu_point_map_.end() )
	{
		delete it->second;
		it = rtu_point_map_.erase(it);
	}
}

SecRtuMap*			SecRtuManager::GetRtuMap( int rtu_map_id )
{
	RTU_POINT_MAP::iterator it = rtu_point_map_.find( rtu_map_id );
	if( it == rtu_point_map_.end() )
		return NULL;

	return it->second;
}


int					SecRtuManager::AddRtuDevice( int rtu_id, char* rtu_name, int map_id, int fep_id, int data_link )
{
	if( rtu_id <= 0 )
		return SEC_PARAM_ERR;

	SecRtuDevice* prtu_dev = GetRtuDevice( rtu_id );
	if( prtu_dev != NULL )
		return SEC_DATA_EXIST;

	SecRtuMap* rtu_map = GetRtuMap( map_id );
	if( rtu_map == NULL )
		return SEC_DATA_NOT_EXIST;

	prtu_dev = new SecRtuDevice(rtu_id, rtu_name, map_id, fep_id, data_link, rtu_map);
	
	rtu_dev_map_.insert(pair<int, SecRtuDevice*>(rtu_id, rtu_dev));

	return SEC_OK;
}

void				SecRtuManager::DelRtuDevice( )
{
	RTU_DEV_MAP::iterator it = rtu_dev_map_.begin();
	while( it != rtu_dev_map_.end() )
	{
		delete it->second;
		it = rtu_dev_map_.erase(it);
	}
}

SecRtuDevice*			SecRtuManager::GetRtuDevice( int rtu_id )
{
	RTU_DEV_MAP::iterator it = rtu_dev_map_.find( rtu_id );
	if( it == rtu_dev_map_.end() )
		return NULL;

	return it->second;
}
