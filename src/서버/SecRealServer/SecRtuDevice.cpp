#include "StdAfx.h"
#include "SecRtuDevice.h"


SecRtuDevice::SecRtuDevice(int rtu_id, char* rtu_name, int map_id, int fep_id, int data_link, SecRtuMap* prtu_map/* = NULL*/)
	:rtu_id_(rtu_id), rtu_map_id_(map_id), fep_id_(fep_id), data_link_(data_link), prtu_map_(prtu_map)
{
	strcpy_s(rtu_name_, rtu_name);
}


SecRtuDevice::~SecRtuDevice(void)
{
}
