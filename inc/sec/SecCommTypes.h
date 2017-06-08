
#ifndef _WS_COMMON_TYPES_H_
#define _WS_COMMON_TYPES_H_

#include <Windows.h>

#ifndef ushort
#define ushort	unsigned short
#endif

#ifndef uint
#define uint	unsigned int
#endif

#ifndef ulong
#define ulong	unsigned long
#endif

// #ifndef BYTE
// #define BYTE	unsigned char
// #endif

#ifndef	R_OK
#define R_OK    4   /* Test for Read permission */
#define W_OK    2   /* Test for Write permission */
#define X_OK    1   /* Test for eXecute permission */
#define F_OK    0   /* Test for existence of File */
#endif

// RETURN CODE
#define	WEMS_OK					0
#define	WEMS_ERR				-1
#define	WEMS_SKIP				-2
#define	WEMS_ERR_XMIT			-3
#define	WEMS_ERR_PROTOCOL		-4
#define	WEMS_ERR_ACK			-5
#define	WEMS_TIMEOUT			-6
#define	WEMS_DB_DISCONNECTED	-7
#define WEMS_SOCK_CLOSE			-8
#define WEMS_SOCK_RCV_SIZE_ERR	-9
#define WEMS_SOCK_TIMEOUT		10
#define WEMS_NOT_OWNDATA		20
//#define WEMS_ALREADY_INIT		30


#define		MAX_PROCNAME_SZ			16
#define		PROC_CONFIG_SZ			32


// COPY NUM 정의
#define	SEC_ONLY							(0)
#define	SEC_ONLY_COPYNUM					(0)



#define WEMS_MAX_NODE_NAME_STR_SZ			16
#define WEMS_MAX_NODE_ADDR_STR_SZ			16
#define WEMS_MAX_NODE_TYPE_STR_SZ			8
#define WEMS_MAX_NODE_RANK_STR_SZ			WEMS_MAX_NODE_TYPE_STR_SZ
#define WEMS_MAX_NODE_GROUP_STR_SZ			WEMS_MAX_NODE_TYPE_STR_SZ
#define WEMS_MAX_NODE_MODE_STR_SZ			WEMS_MAX_NODE_TYPE_STR_SZ


#define WEMS_NODE_NAME_DELIM				"."
#define WEMS_MAX_ESSC_NODE_CNT				8

typedef struct {
	int             type_num;		/**< type number */
	const char      *type_nm;		/**< type name */
} wemsTypeDesc_st;
 
 typedef enum _wemsNodeType_e {
 	WEMS_NT_NONE	= 0x0,
 	WEMS_NT_PMS	= 0x1,
 	WEMS_NT_ESSC	= 0x2,
 	WEMS_NT_ANY		= 0xF,
 } wemsNodeType_e;
 
 static const wemsTypeDesc_st g_wemsNodeTypeDescTbl[] = {
	 { WEMS_NT_NONE		, "NONE"		},
	 { WEMS_NT_PMS		, "PMS"			},
	 { WEMS_NT_ESSC		, "ESSC"		},
	 { WEMS_NT_ANY		, "ANY"			},
 };
  
  typedef enum _wemsNodeRank_e {
  	WEMS_NR_NONE		= 0x0,
  	WEMS_NR_PRIME		= 0x1,
  	WEMS_NR_BACKUP		= 0x2,
  	WEMS_NR_ANY			= 0xF,
  } wemsNodeRank_e;
 
  static const wemsTypeDesc_st g_wemsNodeRankDescTbl[] = {
  	{ WEMS_NR_NONE		, "N"		},
  	{ WEMS_NR_PRIME		, "A"			},
  	{ WEMS_NR_BACKUP	, "B"			},
  	{ WEMS_NR_ANY		, "ANY"			},
  };
  
  static const wemsTypeDesc_st g_wemsNodeRankDescTbl2[] = {
  	{ WEMS_NR_NONE		, "NONE"		},
  	{ WEMS_NR_PRIME		, "ACTIVE"			},
  	{ WEMS_NR_BACKUP	, "STANDBY"			},
  	{ WEMS_NR_ANY		, "ANY"			},
  };

 typedef enum _wemsNodeGroup_e {
 	WEMS_NG_NONE	= 0x00,
 	WEMS_NG_G01		= 0x01,
 	WEMS_NG_G02		= 0x02,
 	WEMS_NG_G03		= 0x03,
 	WEMS_NG_G04		= 0x04,
 	WEMS_NG_G05		= 0x05,
 	WEMS_NG_G06		= 0x06,
 	WEMS_NG_G07		= 0x07,
 	WEMS_NG_G08		= 0x08,
 	WEMS_NG_ANY		= 0x7F, 
 } wemsNodeGroup_e;

 static const wemsTypeDesc_st g_wemsNodeGroupDescTbl[] = {
 	{ WEMS_NG_NONE			,	"NONE"		},
 	{ WEMS_NG_G01			,	"01"			},
 	{ WEMS_NG_G02			,	"02"			},
 	{ WEMS_NG_G03			,	"03"			},
 	{ WEMS_NG_G04			,	"04"			},
 	{ WEMS_NG_G05			,	"05"			},
 	{ WEMS_NG_G06			,	"06"			},
 	{ WEMS_NG_G07			,	"07"			},
 	{ WEMS_NG_G08			,	"08"			},
 	{ WEMS_NG_ANY			,	"ANY"		},
 };
 
 typedef enum _wemsNodeMode_e{
 	WEMS_NM_NONE		= 0x00,
 	WEMS_NM_SINGLE		= 0x01,
 	WEMS_NM_DUAL		= 0x02,
 	WEMS_NM_ANY			= 0x7F
 }wemsNodeMode_e;
 
 static const wemsTypeDesc_st g_wemsNodeModeDescTbl[] = {
 	{ WEMS_NM_NONE			,	"NONE"		},
 	{ WEMS_NM_SINGLE		,	"single"			},
 	{ WEMS_NM_DUAL			,	"dual"			},
 	{ WEMS_NM_ANY			,	"any"			},
 };


// #define WEMS_MSGMD_PROCNM				"wemsmsgmd"
// #define WEMS_SYSMD_PROCNM				"wemssysmd"
// #define WEMS_SCHMD_PROCNM				"wemsschmd"
// #define WEMS_RTPD_PROCNM				"wemsrtpd"
// #define WEMS_DACP_PROCNM				"wemsdacp"
// #define WEMS_RDBPD_PROCNM				"wemsrdbpd"
// 
// static const char*	g_szWemsMsgmd_procname		= WEMS_MSGMD_PROCNM;


#define NODENAME_EQUAL(a,b)	( ((a)->eNodeType==(b)->eNodeType) && ((a)->eNodeRank==(b)->eNodeRank) && ((a)->eNodeGroup==(b)->eNodeGroup) ? 1 : 0)

 typedef enum _wemsNodeStatus_e {
	 WEMS_NS_NONE		= 0x00,
	 WEMS_NS_INIT		= 0x01,
	 WEMS_NS_ONLINE		= 0x02,
	 WEMS_NS_OFFLINE		= 0x03,
	 WEMS_NS_TAKEOVER	= 0x04,
	 WEMS_NS_SWITCHOVER	= 0x05,
	 WEMS_NS_UNKNOWN	= 0xFF
 } wemsNodeStatus_e;

 static const wemsTypeDesc_st g_wemsNodeStatusDescTbl[] = {
	 { WEMS_NS_NONE		, "NONE"		},
	 { WEMS_NS_INIT		, "INIT"		},
	 { WEMS_NS_ONLINE		, "ONLINE"		},
	 { WEMS_NS_OFFLINE	, "OFFLINE"	},
	 { WEMS_NS_TAKEOVER	, "TAKEOVER"	},
	 { WEMS_NS_SWITCHOVER	, "SWITCHOVER"	},
	 { WEMS_NS_UNKNOWN	, "UNKNOWN"	},
 };

typedef struct {
	ushort	usNodeCode;
	ushort	usCopyNo;
	char	szProcName[MAX_PROCNAME_SZ];
}wemsProcInfo_st;


 typedef	struct
 {
 	wemsNodeType_e		eNodeType	:4;
 	wemsNodeRank_e		eNodeRank	:4;
 	wemsNodeGroup_e		eNodeGroup	:8;
 } wemsNodeName_st;
 
 typedef struct _wemsProcName_st
 {
 	int				iCopy;
 	char			szProcName[MAX_PROCNAME_SZ];
 }wemsProcName_st;
 
 typedef struct _wemsGPN_st
 {
 	wemsNodeName_st			stNodeName;
 	wemsProcName_st			stProcName;
 }wemsGPN_st;


typedef struct  
{
	wemsProcInfo_st	SrcProc;
	wemsProcInfo_st DestProc;
	ushort			usTotPktCnt;
	ushort			usPktIdx;
	ushort			usPktLength;
}wemsPacketHeader_st;

typedef struct
{
	uint		uiTime;
	uint		uiMillitm;
	ushort		usRequestFc;
	ushort		usResqonseFc;
	uint		usCount;
}wemsDataHeader_st;


#define	DEF_MSG_HOSTNAME			"localhost"
#define	DEF_MSG_PORTNO				37156

#define	DEF_SLAVE_TCP_PORTNO		502
#define	DEF_HMI_TCP_PORTNO			33033

#define	ENVVAR_DEFAULT_INIFPATH		"__WEMS_DEFAULT_INIFPATH__"
#define	ENVVAR_DEFAULT_TESTMODE		"__WEMS_DEFAULT_TESTMODE__"
#define	ENVVAR_MY_SYSTEM_NO			"__WEMS_MY_SYSTEM_NO__"
#define	DEFAULT_INIFPATH			"config/wemscommon.ini"

#define	SYSTEM_TEST_MODE_YES		1		// <<주의>> test 모드로써, 실제 계측을 하지 않고 임의의 데이터를 계측한 것처럼 하여 올려보낸다.
#define	SYSTEM_TEST_MODE_NO			0		// 정상 계측 모드. 실제 계측하여 올려보낸다. 

#define	DEF_XMIT_TIMEOUTMSEC	3000

#define	CHP_LOG_DIRECTION_NONE		00000000
#define	CHP_LOG_DIRECTION_CONSOLE	00000001
#define	CHP_LOG_DIRECTION_FILE		00000010
#define	DEF_LOG_DIR					"./log"
#define	DEF_LOG_LEVEL				 1					// 0:off ~ 4:most detail
#define	DEF_LOG_MAX_FILESIZE		 20					// 2 MBytes


#define	DEF_LOG_USE_LOCKING_Y		'Y'
#define	DEF_LOG_USE_LOCKING_N		'N'
#define	DEF_LOG_USE_LOCKING			1
#define	DEF_LOG_UNUSE_LOCKING		0

#define	CHP_LOG_BACKUP_METHOD_BACKUP	0
#define	CHP_LOG_BACKUP_METHOD_DAY		1
#define	CHP_LOG_BACKUP_METHOD_PROC		2
#define	CHP_LOG_BACKUP_METHOD_DEFAULT	CHP_LOG_BACKUP_METHOD_DAY

#define	CHP_EVENT_SAVE_TYPE_BINARY		1
#define	CHP_EVENT_SAVE_TYPE_CSV			10

#define	CHP_EVENT_SAVE_DIR_METHOD_1DIR		0
#define	CHP_EVENT_SAVE_DIR_METHOD_DAY		1
#define	CHP_EVENT_SAVE_DIR_METHOD_DEFAULT	CHP_EVENT_SAVE_DIR_METHOD_DAY

#define	CHP_EVENT_IN_TEST_MODE_YES		1
#define	CHP_EVENT_IN_TEST_MODE_NO		0
#define	CHP_EVENT_IN_TEST_MODE_DEFAULT	CHP_EVENT_IN_TEST_MODE_NO

#define	DEF_EVENT_SAVE_BASEDIR			"./event"
#define	DEF_EVENT_SAVE_BASEFILENAME		"event"

#define	DEF_BYTE_ORDERING_MSB	0
#define	DEF_BYTE_ORDERING_LSB	1

#define	DEF_WORD_SWAP_YES		1
#define	DEF_WORD_SWAP_NO		0

#define	DEF_QUANTITY_OF_REGISTERS	56		// 485 에서 한번에 읽을 수 있는 Register 갯수. (default 값)

#define	DEF_DAC_OPENTYPE_SERIAL		"Serial"
#define	DEF_DAC_OPENTYPE_TCP		"TCP"
#define	DEF_DAC_OPENTYPE_UDP		"UDP"
#define	DEF_DAC_COMMTYPE_SERIAL		"Serial"
#define	DEF_DAC_COMMTYPE_TCP		"TCP"
#define	DEF_DAC_PROTOCOL_MB485		"Modbus485"
#define	DEF_DAC_PROTOCOL_MAXIM		"MAXIM"
#define	DEF_DAC_PROTOCOL_USB232		"USB232"
#define	DEF_DAC_PROTOCOL_INET		"I-NET"
#define	DEF_DAC_PROTOCOL_MEGATEC	"MegaTec"
#define	DEF_DAC_PROTOCOL_DEVC		"DEVC"
#define	DEF_DAC_PROTOCOL_COGEN		"Cogen"
#define	DEF_DAC_PROTOCOL_MASTERK	"Master-K"
#define	DEF_DAC_PROTOCOL_ADVDAQ		"AdvDAQ"
#define	DEF_DAC_PROTOCOL_ADVNAVI	"AdvNAVI"
#define	DEF_DAC_PROTOCOL_ELT		"ELT"		// 항온항습기 자체 프로토콜
#define	DEF_DAC_PROTOCOL_NETSNMP	"SNMP"

#define	DEF_UDP_HOSTNAME			"localhost"
#define	DEF_UDP_PORTNO				161
#define	DEF_UDP_TIMEOUTMSEC			1000

#define	DEF_DAC_MAPTYPE				0
#define	DEF_TCP_HOSTNAME			"localhost"
//#define	DEF_TCP_PORTNO				502
#define	DEF_TCP_TIMEOUTMSEC			1000
#define	DEF_TCP_TRANSACTIONID_MAX	10000

#define	DEF_SERIAL_PORT				3
#define	DEF_SERIAL_BAUDRATE			9600
#define	DEF_SERIAL_PARITY			0
#define	DEF_SERIAL_STOPBITS			1
#define	DEF_SERIAL_DATABITS			8
#define	DEF_SERIAL_TIMEOUTMSEC		500		// unit:msec

#define	POINTID_TURNONOFF_ALRAM_LIGHT	7009

typedef	struct	{
	char	hostname[32];	// for ADO
	int		portno;			// for ADO
	char	userid[32];		// for ADO, ODBC
	char	passwd[32];		// for ADO, ODBC
	char	dbname[32];		// for ADO
	char	charset[32];
} _DBINFO;
typedef	struct	{
	_DBINFO		rdb;
	_DBINFO		hdb;
} COMMON_DBINFO;

typedef	struct	{
	int		portno;			// Portno, 1:COM1,2:COM2,3:COM3,...
	int		baudrate;		// 110,300,600,1200,2400,4800,9600,14400,19200,38400,56000,57600,115200,128000,256000
	int		parity;			// Parity, 1:Yes, 0:No
	int		stopbits;		// Stop Bits
	int		databits;		// Data Bits
	int		timeoutmsec;	// Serial 통신 수신시 첫 ch 가 올때까지 기다리는 시간. unit:msec
	DWORD	ReadIntervalTimeout;
	DWORD	ReadTotalTimeoutMultiplier;
	DWORD	ReadTotalTimeoutConstant;
	DWORD	WriteTotalTimeoutMultiplier;
	DWORD	WriteTotalTimeoutConstant;
} COMMON_SERIALINFO;

typedef	struct	{
	char	hostname[24];
	int		portno;
	int		timeoutmsec;	// Timeout Second (unit=millisecond)
	int		transactionid;	// Query/Response, Send value increased by 1 and respond same value
	int		protocolid;		// Fixed value(0x0000)
} COMMON_TCPIPINFO;

typedef struct  {
	char	hostname[24];
	int		portno;
	int		timeoutmsec;	// Timeout Second (unit=millisecond)
} COMMON_UDPINFO;

typedef	struct	{
	int		id;
	int		destination_address;
	int		maptype;
	int		scaninterval;	// Scan 주기. g_idcount x g_readintervalmsec/1000 보다 작지않은 값으로 세팅할 것.
	char	opentype[24];	// Comm Connection Method : 'Serial'/'TCP'
	char	commtype[24];	// Modebus 'Serial'/'TCP'
	struct	{
		int		common;
		int		ba01h;		// 00000 : Read Coil Status
		int		ba02h;		// 10000 : Read Input Status
		int		ba03h;		// 40001 : Read Holding Register
		int		ba04h;		// 30001 : Read Input   Register
		int		ba05h;		// 00001 : Force Single Coil
		int		ba06h;		// 40001 : Preset Single   Register
		int		ba10h;		// 40001 : Preset Multiple Register
	} base_address;
} COMMON_DACINFO;


// formula 용 버퍼 (ex> Adam : 4~20 mA --> 0~30 )
typedef	struct	{
	double	raw_meas_min;
	double	raw_meas_max;
	double	real_meas_min;
	double	real_meas_max;
} ai_formula_st;


// Device 별 프로세스 (ex>wemsmb485) 가 wemsdacp 에게 등록을 요청할때 전송하는 자료구조
typedef	struct	_dac_regist_st{
	int		id;		// DAC ID
	char	procname[MAX_PROCNAME_SZ]; // 프로세스 이름으로 전송하는 것으로 수정
	int		cpno;
	int		state;
	int		maptype;
	char	protocol[20];
	char	opentype[20];
	char	commtype[20];

	// for "Serial" commtype
	char	comport[20];
	int		baudrate;
	int		stopbits;
	int		databits;
	int		parity;

	// for "TCP/IP" commtype
	char	tcphostname[32];
	int		tcpportno;

	time_t	tmconnect;

	int		scaninterval;	// Scan 주기 (단위:sec)
} dac_regist_st;


// Device 별 프로세스 (ex>wemsmb485) 가 wemsdacp 에게 등록 해제를 요청할때 전송하는 자료구조
typedef	struct	_dac_unregist_st{
	int		id;		// DAC ID
	char	procname[MAX_PROCNAME_SZ]; // 프로세스 이름으로 전송하는 것으로 수정
	int		cpno;
	int		state;
	time_t	tmdisconnect;
} dac_unregist_st;


// 동기화할 PDB 전송 버퍼 (FC_UPDATE_PDB) : UA's wemsemsrcv --> DA's wemsrtpd
typedef	struct	{
	int		pdbno;
	char	pdbdata[4];		// 필드명만 사용하러고 size 를 4 로 함.
} wmsg_update_pdb_st;

// 동기화할 PDB 전송 버퍼 (FC_REQUEST_SYNC_PDB) : DA's wemsrtpd --> UA's wemsrtpd
typedef	wmsg_update_pdb_st	wmsg_req_sync_pdb_st;

/*----------------------------------------------------------------------------*/
/*	Event Definitions														  */
/*----------------------------------------------------------------------------*/
#define	WEMS_EVENT_TYPE_CONTROL				'A'
#define	WEMS_EVENT_TYPE_CHANGE_DI			'B'
#define	WEMS_EVENT_TYPE_LIMIT_EXCEED		'C'
#define	WEMS_EVENT_TYPE_COMM_ERROR			'D'
#define	WEMS_EVENT_TYPE_SLAVE_RECV_CONTROL	'E'

#define	DEF_NOTIFY_TXRX_TX			1		// Tx
#define	DEF_NOTIFY_TXRX_RX			2		// Rx


#define	DEF_DAC_STATE_OFFLINE		0		// OffLine
#define	DEF_DAC_STATE_ONLINE		1		// OnLine

#define	WEMS_MAX_COPYNO				99


/*------------------------------------------------------------------ for wemsmb485's common format */
#define	DEF_FORMAT_FOR_COMMON_SHORT				"short"		// short
#define	DEF_FORMAT_FOR_COMMON_USHORT			"ushort"	// ushort
#define	DEF_FORMAT_FOR_COMMON_INT				"int"		// int
#define	DEF_FORMAT_FOR_COMMON_UINT				"uint"		// uint
#define	DEF_FORMAT_FOR_COMMON_LONG				"long"		// long
#define	DEF_FORMAT_FOR_COMMON_ULONG				"ulong"		// ulong
#define	DEF_FORMAT_FOR_COMMON_FLOAT				"float"		// float
#define	DEF_FORMAT_FOR_COMMON_DOUBLE			"double"	// double

#define	DEF_FORMAT_FOR_DEVC_BATT				"BATT"		// BATT


/*------------------------------------------------------------------ for Common */
// 나중에는 bitmap 체크할 위치를 모두 이걸로 사용하도록 보완할 예정. 2014-02-26(수) nold0001@nate.com
#define	DEF_BITMAP_COMMON_00			0x0001		//
#define	DEF_BITMAP_COMMON_01			0x0002		//
#define	DEF_BITMAP_COMMON_02			0x0004		//
#define	DEF_BITMAP_COMMON_03			0x0008		//
#define	DEF_BITMAP_COMMON_04			0x0010		//
#define	DEF_BITMAP_COMMON_05			0x0020		//
#define	DEF_BITMAP_COMMON_06			0x0040		//
#define	DEF_BITMAP_COMMON_07			0x0080		//
#define	DEF_BITMAP_COMMON_08			0x0100		//
#define	DEF_BITMAP_COMMON_09			0x0200		//
#define	DEF_BITMAP_COMMON_10			0x0400		//
#define	DEF_BITMAP_COMMON_11			0x0800		//
#define	DEF_BITMAP_COMMON_12			0x1000		//
#define	DEF_BITMAP_COMMON_13			0x2000		//
#define	DEF_BITMAP_COMMON_14			0x4000		//
#define	DEF_BITMAP_COMMON_15			0x8000		//


#if 0
typedef struct
{
	uint     uiTime;
	ushort   sRequestFc;
	ushort   sResqonseFc;
	uint     usCount;
}wemsDataHeader_st;


typedef struct
{
	BYTE    ucNodeCode;		
	BYTE    ucActCode;
	ushort  usTotPktCnt;
	ushort  usPktIdx;
	BYTE    ucSequence;
	BYTE    ucCompress;
	ushort  usPktLength;
}wemsPacketHeader_st;
#endif

typedef struct
{
	wemsDataHeader_st   stDataHeader;
	char				*pData;
}wemsDataPacket_st;


typedef struct
{
	wemsPacketHeader_st     stPacketHeader;
	char            *pData;			// *pData -> pData
} wemsPacket_st;

#define WEMSTCP_PACKET_SZ				4096
#define WEMSTCP_HEADER_SZ				sizeof( wemsPacketHeader_st )
#define WEMSTCP_DATA_SZ					(WEMSTCP_PACKET_SZ - WEMSTCP_HEADER_SZ) // 전체 패킷 사이즈(4096) - 패킷해더사이즈

#define WEMSDATA_HEADER_SZ				sizeof( wemsDataHeader_st )

#define WEMSTCP_RCV_TIMEOUT				3000 //3초



#define L(n,m) (n>>(8-m)) 
#define R(n,m) (((n<<(8-m))&255) >> (8-m))

#define L64(n,m) (n>>(6-m)) 
#define R64(n,m) (((n<<(6-m))&63) >> (6-m))

#define	CHP_PIPE_BUFF_SIZE	8192


#ifndef	__func__
#define	__func__	__FUNCTION__
#endif

#endif _WS_COMMON_H_