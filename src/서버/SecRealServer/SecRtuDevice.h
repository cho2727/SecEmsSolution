#pragma once

class SecRtuMap;
class SecRtuDevice
{
public:
	SecRtuDevice(int rtu_id, char* rtu_name, int map_id, int fep_id, int data_link, SecRtuMap* prtu_map = NULL);
	~SecRtuDevice(void);

	int			RtuID() { return rtu_id_; }
	int			DataLink() { return data_link_; }
	int			FepID() { return fep_id_; }
	int			RtuMapID() { return rtu_map_id_; }

	const char*		RtuName() { return rtu_name_; }
private:
	// 통신 상태 및 
	int			rtu_id_;
	int			data_link_;
	int			fep_id_;		// FepCopyNo
	int			rtu_map_id_;

	char		rtu_name_[64];
	SecRtuMap*	prtu_map_;
};

