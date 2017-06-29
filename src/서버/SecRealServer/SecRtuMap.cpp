#include "StdAfx.h"
#include "SecRtuMap.h"



struct find_point_index
{
	find_point_index(int iPointType, int iPointIndex)
		: point_type_(iPointType), point_index_(iPointIndex)
	{

	}

	bool operator()(const pair<int, PointIndex_st*>& rhs)
	{
		bool bFlag = false;

		if(rhs.second->point_type == point_type_ && rhs.second->point_index == point_index_)
			bFlag = true;

		return bFlag;
	}

private:
	int point_type_;
	int point_index_;
};


SecRtuMap::SecRtuMap( int map_id, int type_id, char* map_name, char* type_name, char* kind_name, int bi_cnt, int bo_cnt, int ai_cnt, int ao_cnt, int ci_cnt )
	: rtu_map_id_(map_id),  rtu_type_id_(type_id), bi_count_(bi_cnt), bo_count_(bo_cnt), ai_count_(ai_cnt), ao_count_(ao_cnt), ci_count_(ci_cnt)
{
	strcpy_s(rtu_map_name_, map_name);
	strcpy_s(rtu_type_name_, type_name);
	strcpy_s(rtu_kind_name_, kind_name);
}


SecRtuMap::~SecRtuMap(void)
{
}

int					SecRtuMap::AddPointIndex( int point_type, PointIndex_st* point_index )
{
	int iRet = 0;
	if( PointType_e::PT_BI > point_type || PointType_e::PT_CNT < point_type )
		return SEC_PARAM_ERR;

	PointIndex_st* pData = GetPointIndex( point_type, point_index->point_index );
	if(pData != NULL)
		return SEC_DATA_EXIST;

	point_index_map_.insert(pair<int, PointIndex_st*> (point_type, point_index));

	return iRet;
}

void					SecRtuMap::DelPointIndex( )
{
	PointIndexMap::iterator it = point_index_map_.begin();
	while( it != point_index_map_.end() )
	{
		delete it->second;
		it = point_index_map_.erase(it);
	}
}

PointIndex_st*		SecRtuMap::GetPointIndex( int point_type, int index )
{
	PointIndexMapPair		pos_range;
	pos_range = point_index_map_.equal_range( point_type );
	
	PointIndexMap::iterator it = find_if(pos_range.first, pos_range.second, find_point_index(point_type, index));

	if(it == point_index_map_.end() || it->second->point_index != index)
		return NULL;

	return it->second;

	return NULL;
}