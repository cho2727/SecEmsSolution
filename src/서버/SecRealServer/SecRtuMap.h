#pragma once

typedef struct _PointIdex_st
{
	int		rtu_map_id;
	int		point_type;
	int		point_index;
	int		circuit_no;

	int		alarm_type;
	int		alarm_code_class;

	int		state_grp_id;
	float	normal_value;
	float	abnormal_value;

	int		point_scale;
	int		point_limit;
	int		point_minmax;
	int		command_point_index;

	int		alarm;
	int		important;
	
	int		hist_flag;
}PointIndex_st;

class SecRtuMap
{
public:
	typedef multimap<int, PointIndex_st*> PointIndexMap;
	typedef pair<PointIndexMap::iterator, PointIndexMap::iterator> PointIndexMapPair;
public:
	SecRtuMap( int map_id, int type_id, char* map_name, char* type_name, char* kind_name, int bi_cnt, int bo_cnt, int ai_cnt, int ao_cnt, int ci_cnt );
	~SecRtuMap(void);

	int					RtuMapID() { return rtu_map_id_; }
	int					RtuTypeID() { return rtu_type_id_; }

	const char*			RtuMapName() { return rtu_map_name_; }	
	const char*			RtuTypeName() { return rtu_type_name_; }	
	const char*			RtuKindName() { return rtu_kind_name_; }

	int					AddPointIndex( int point_type, PointIndex_st* point_index );
	void				DelPointIndex( );
	PointIndex_st*		GetPointIndex( int point_type, int index );

private:
	int			rtu_map_id_;
	int			rtu_type_id_;

	char	rtu_map_name_[64];
	char	rtu_type_name_[32];
	char	rtu_kind_name_[32];

	int		bi_count_;
	int		bo_count_;
	int		ai_count_;
	int		ao_count_;
	int		ci_count_;

	PointIndexMap	point_index_map_;

};

