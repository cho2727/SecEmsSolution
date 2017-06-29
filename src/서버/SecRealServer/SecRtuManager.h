#pragma once

class SecRtuMap;
class SecRtuDevice;
class SecRtuManager
{
public:
	typedef map<int, SecRtuMap*> RTU_POINT_MAP;
	typedef map<int, SecRtuDevice*> RTU_DEV_MAP;
private:
	SecRtuManager(void);
public:
	~SecRtuManager(void);

	static SecRtuManager* GetInstance();

	int					RtuLoad();
	int					RtuMapLoad();
	int					RtuDeviceLoad();
	int					AddRtuMap( int map_id, int type_id, char* map_name, char* type_name, char* kind_name, int bi_cnt, int bo_cnt, int ai_cnt, int ao_cnt, int ci_cnt );
	void				DelRtuMap( );
	SecRtuMap*			GetRtuMap( int rtu_map_id );

	int					AddRtuDevice( int rtu_id, char* rtu_name, int map_id, int fep_id, int data_link );
	void				DelRtuDevice( );
	SecRtuDevice*			GetRtuDevice( int rtu_id );

private:
	RTU_POINT_MAP		rtu_point_map_;
	RTU_DEV_MAP			rtu_dev_map_;
};

