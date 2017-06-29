#ifndef _SEC_REAL_DATA_TYPE_H_
#define _SEC_REAL_DATA_TYPE_H_

#include "sec/SecCommTypes.h"
typedef struct _ShardRtuDevice
{
	int			rtu_id;
	byte		comm_type;
	int			rtu_map_id;
	int			start_index_bi;
	int			start_index_bo;
	int			start_index_ai;
	int			start_index_ao;
	int			start_index_cnt;
	byte		comm_state;
	ushort		comm_total_cnt;
	ushort		comm_sucess_cnt;
	ushort		comm_fail_cnt;
	ushort		comm_no_response_cnt;
}ShardRtuDevice;


typedef	struct _ShardBinaryInput
{
	int			rtu_id;
	ushort		point_index;
	byte		point_value;
	ushort		point_tlq;
	uint		point_uptime;
}ShardBinaryInput;

typedef	struct _ShardBinaryOutput
{
	int			rtu_id;
	ushort		point_index;
	ushort		point_tlq;
	uint		point_uptime;
}ShardBinaryOutput;


typedef	struct _ShardAnalogInput
{
	int			rtu_id;
	ushort		point_index;
	double		point_value;
	ushort		point_tlq;
	uint		point_uptime;
}ShardAnalogInput;



typedef	struct _ShardAnalogOutput
{
	int			rtu_id;
	ushort		point_index;
	double		point_value;
	ushort		point_tlq;
	uint		point_uptime;
}ShardAnalogOutput;

typedef	struct _ShardCounter
{
	int			rtu_id;
	ushort		point_index;
	ushort		point_value;
	ushort		point_tlq;
	uint		point_uptime;
}ShardCounter;


#endif