#ifndef __SEC_ERRORCODE_H__
#define __SEC_ERRORCODE_H__

const int SEC_OK						= 0;						// 정상처리
const int SEC_ERR						= -1001;
const int SEC_SKIP						= -1002;
const int SEC_ERR_XMIT					= -1003;
const int SEC_ERR_PROTOCOL				= -1004;
const int SEC_ERR_ACK					= -1005;
const int SEC_TIMEOUT					= -1006;
const int SEC_DB_DISCONNECTED			= -1007;
const int SEC_SOCK_CLOSE				= -1008;
const int SEC_SOCK_RCV_SIZE_ERR			= -1009;
const int SEC_SOCK_TIMEOUT				= -1011;
const int SEC_NOT_OWNDATA				= -1012;
const int SEC_SOCK_NOT_CONNECT			= -1013;
const int SEC_SOCK_EXCEPTION			= -1014;
const int SEC_DATA_EXIST				= -1015;
const int SEC_DATA_NOT_EXIST			= -1016;
const int SEC_PARAM_ERR					= -1017;

const int SEC_CONFIG_INIT_FAIL			= -2011;
const int SEC_CONFIG_LOAD_FAIL			= -2012;
const int SEC_CONFIG_DOC_FAIL			= -2013;
const int SEC_CONFIG_SEL_FAIL			= -2014;
const int SEC_CONFIG_EXCEPTION			= -2015;
const int SEC_CONFIG_NOT_DATA			= -2016;


#endif