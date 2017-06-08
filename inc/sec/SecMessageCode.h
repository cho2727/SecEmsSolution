#ifndef __SEC_MESSAGE_CODE_H__
#define __SEC_MESSAGE_CODE_H__

/** 프로세스 메시지 코드 */
#define		FC_PROC_REG_REQS		(0x2001)		/*	프로세스 등록 요청	*/
#define		FC_PROC_TEST_REQS		(0x2002)		/*		*/
#define		FC_PROC_START_REQS		(0x2003)
#define		FC_PROC_STOP_REQS		(0x2004)

#define		FC_PROC_KILL_REQS		(0x2005)

#define		FC_PROC_SINGLE_START	(0x2006)
#define		FC_PROC_SINGLE_STOP		(0x2007)
#define		FC_PROC_SINGLE_KILL		(0x2008)
#define		FC_PROC_LOG_CHANGE		(0x2009)
#define		FC_PROC_WORK_CHECK		(0x200a)
#define		FC_PROC_SYSTEM_CHECK	(0x200b)
#define		FC_PROC_RELOAD_CONFIG	(0x200c)

/** 노드 메시지 코드 */
#define		FC_NODE_INFO_REQS		(0x3001)		//< 노드 정보 요청 -> 타노드(ESSC, PMS.B) --> PMS.A || ESSC.B -> ESSC.A
#define		FC_NODE_INFO_RESP		(0x3002)		//< 노드 정보 전송 -> PMS.A --> 타노드(ESSC, PMS.B) || ESSC.A -> ESSC.B
#define		FC_NODE_INFO_UPDATE		(0x3003)		//< 자신의 노드 정보 전송(업데이트용) -> 타노드(ESSC, PMS.B) --> PMS.A || ESSC.B -> ESSC.A
#define		FC_NODE_INFO_TOTAL		(0x3004)		//< 전체 노드 정보 전송(업데이트용) - PMS.A --> 전체 연결된 노드
#define		FC_NODE_INFO_CHANGE		(0x3005)
#define		FC_NODE_INFO_SWITCH		(0x3006)
#define		FC_NODE_CONN_FAIL		(0x3007)
#define		FC_NODE_SOCKET_CHECK	(0x3008)
#define		FC_NODE_ALIVE_CHECK		(0x3009)
#define		FC_NODE_CONN_COMP		(0x300a)
#define		FC_NODE_RANK_CHANGE		(0x300b)


#define		FC_DMC_DB_UPDATE				0x4101		// DMC Pdb --> RDB (DMC 정보를 RDB 에 반영)
#define		FC_UPDATE_PDB					0x4201		// wemsemsrcv --> DA's wemsrtpd (상위EMS 에서온 Algorithm 용 제어데이터 pdb 를 DA 에게 전달)
#define		FC_SEND_RTCONTROL_OUT			0x4202		// DA wemsapmd --> DA wemsrtpd
#define		FC_SEND_AIREALTIME_OUT			0x4203		// DA wemsrtpd  --> UA wemsrtpd --> UA wemsemssnd --> U EMS
#define		FC_SEND_SCHEDULING_OUT			0x4204		// DA wemsapmd --> DA wemsrtpd
#define		FC_SEND_AISCHEDULING_OUT		0x4205		// UA wemsappmd --> UA wemsrtpd --> UA wemsemssnd --> U EMS
#define		FC_SEND_FORECAST_OUT			0x4206		// DA wemsapmd --> DA wemsrtpd
#define		FC_SEND_AIFORECAST_OUT			0x4207		// UA wemsappmd --> UA wemsrtpd --> UA wemsemssnd --> U EMS
#define		FC_ALRAM_SYNC_PDB				0x4208		// DA 의 wemsrtpd 가 매초 UA 의 wemsrtpd 에게 pdb (현재는 dmc 만) 동기화 명령을 보내라는 알람.
#define		FC_REQUEST_SYNC_PDB				0x4209		// DA 의 wemsrtpd 가 UA 의 wemsrtpd 에게 pdb 를 주면서 동기화 하라는 명령. (wemsschmd 에게 FC_ALRAM_SYNC_PDB 를 받은 후 실제 동기화할 pdb 를 보내는 명령)
#define		FC_MW_TRIGGER_UPDATE			0x4210		// 30초마다 update
#define		FC_EMERGENCY_OPER_NOTIFY		0x4211		// 비상운전 돌입시 DA 의 rtinput.reserved5 에 1 로 세팅
#define		FC_UPDATE_STAT_DATA				0x4212		// 통계데이터를 갱신하라는 요청
#define		FC_ARRANGE_RESOURCE				0x4213		// 리소스를 정리하라는 요청
#define		FC_ASSEMMEA_DB_UPDATE			0x4214		// ASSEM MEASURE PDB --> RDB(


#define		FC_DAC_REGIST					(0x5001)		// Regist DAC per Device to DAC Table
#define		FC_DAC_REGIST_RES_OK			(0x5002)		// ok result response of FC_DAC_REGIST
#define		FC_DAC_UNREGIST					(0x5003)		// UnRegist DAC per Device to DAC Table
#define		FC_DAC_UNREGIST_RES_OK			(0x5004)		// ok result response of FC_DAC_UNREGIST
#define		FC_DAC_RUN_SCAN_ALL				(0x5005)		// DAC 들에게 Scan 을 날리기위해 등록한 스케줄
#define		FC_SCHD_BI						(0x5301)		// Binary Input Data      (read measure 요청)
#define		FC_SCAN_BI						(0x5302)		// Binary Input Scan Data (read measure 요청)
#define		FC_SCHD_AI						(0x5401)		// Analog Input Data      (read measure 요청)
#define		FC_SCAN_AI						(0x5402)		// Analog Input Scan Data (read measure 요청)
#define		FC_BI_DATA						(0x5501)		// measure read 결과데이터 전송
#define		FC_AI_DATA						(0x5601)		// measure read 결과데이터 전송
#define		FC_MEASUREMENT_DATA				(0x5602)		// measure 데이터를 DB 에 update 하라는 요청
#define		FC_EVENT_GENERATION				(0X5603)		// 계측 프로세스들이 wemsrtpd 에게 Event 가 발생했다고 알림. (wemsrtpd 는 wemsrdbpd 에게 bypass 함)
#define		FC_CTRL_BO						(0x5701)		// 제어명령 (BO/DO)
#define		FC_EMS_UPDATE_SEND				(0X5702)		// EMS 에게 계측데이터를 전달할 시간이 되었다는 알람 메시지
#define		FC_EMS_UI_CONTROL				(0X5703)		// EMS 의 UI Control 데이터 (exist in rdb) 를 하위A 에게 전달하는 메시지
#define		FC_CTRL_AO						(0x5704)		// 제어명령 (AO)
#define     FC_CTRL_OK						(0x5705)
#define     FC_CTRL_NO						(0x5706)
// 테스트용 Function Codes <<주의>>
#define		FC_TEST_DAC_RUN_SCAN_ALL		(0x5999)		// DAC 들에게 Scan 을 날리기위해 등록한 스케줄 (실제 계측이 아니고, 임의의 데이터를 마치 계측한 것처럼 가장하여 올려보낼것임)



#define		FC_PDB_FILE_REQS				(0x6001)			// PDB 파일 요청	PDBID,					wemssysmd -> wemssysmd => 파일을 로딩하여 전송.
#define		FC_PDB_FILE_RESP				(0x6002)			// PDB 파일 응답    PDB 파일 전송			wemssysmd -> wemssysmd => 수신된 정보 파일 저장
#define		FC_SHM_INFO_REQS				(0x6003)			// PDB(SHM) ID에 따른 공유메모리 요청
#define		FC_SHM_INFO_REPS				(0x6004)			// PDB(SHM) ID에 따른 공유메모리 요청에 대한 응답
#define		FC_PID_SHM_INFO_REQS			(0x6005)			// PDB(SHM) ID + PID 에 따른 공유메모리 요청
#define		FC_PID_SHM_INFO_REPS			(0x6006)			// PDB(SHM) ID + PID 에 따른 공유메모리 요청에 대한 응답


/** 스케쥴 메시지 코드 */
#define     FC_WEMS_SCHD_ADD        (0x1201)        /*  스케쥴링 데이터 추가            PROC SERVER SCHMD   */
#define     FC_WEMS_SCHD_DELETE     (0x1202)        /*  스케쥴링 데이터 삭제            PROC SERVER SCHMD   */
#define     FC_WEMS_SCHD_QUERY      (0x1203)        /*  스케쥴링 데이터 요청            PROC SERVER SCHMD   */
#define		FC_WEMS_SCHD_STOP		(0x1204)		/*  스케쥴링 종료										*/

//#define		FC_NODE_INFO_REPS		(0x3002)	


#define WEMS_MAX_COMM_MESG_STR_SZ			32
typedef struct _wemsCommMessage_st
{
	uint		uiMsgCode;
	char		szMsgData[WEMS_MAX_COMM_MESG_STR_SZ];
}wemsCommMessage_st;



static const wemsCommMessage_st arrCommMsg1[] =
{
	{FC_WEMS_SCHD_ADD,				"FC_WEMS_SCHD_ADD"},
	{FC_WEMS_SCHD_DELETE,			"FC_WEMS_SCHD_DELETE"},
	{FC_WEMS_SCHD_QUERY,			"FC_WEMS_SCHD_QUERY"},
	{FC_WEMS_SCHD_STOP,				"FC_WEMS_SCHD_STOP"}
};

static const wemsCommMessage_st arrCommMsg2[] =
{
	{FC_PROC_REG_REQS,				"FC_PROC_REG_REQS"},
	{FC_PROC_TEST_REQS,				"FC_PROC_TEST_REQS"},
	{FC_PROC_START_REQS,			"FC_PROC_START_REQS"},
	{FC_PROC_STOP_REQS,				"FC_PROC_STOP_REQS"},
	{FC_PROC_KILL_REQS,				"FC_PROC_KILL_REQS"},
	{FC_PROC_SINGLE_START,			"FC_PROC_SINGLE_START"},
	{FC_PROC_SINGLE_STOP,			"FC_PROC_SINGLE_STOP"},
	{FC_PROC_SINGLE_KILL,			"FC_PROC_SINGLE_KILL"},
	{FC_PROC_LOG_CHANGE,			"FC_PROC_LOG_CHANGE"},
	{FC_PROC_WORK_CHECK,			"FC_PROC_WORK_CHECK"},
	{FC_PROC_SYSTEM_CHECK,			"FC_PROC_SYSTEM_CHECK"},
	{FC_PROC_RELOAD_CONFIG,			"FC_PROC_RELOAD_CONFIG"}
};

static const wemsCommMessage_st arrCommMsg3[] =
{
	{FC_NODE_INFO_REQS,				"FC_NODE_INFO_REQS"},
	{FC_NODE_INFO_RESP,				"FC_NODE_INFO_RESP"},
	{FC_NODE_INFO_UPDATE,			"FC_NODE_INFO_UPDATE"},
	{FC_NODE_INFO_TOTAL,			"FC_NODE_INFO_TOTAL"},
	{FC_NODE_INFO_CHANGE,			"FC_NODE_INFO_CHANGE"},
	{FC_NODE_INFO_SWITCH,			"FC_NODE_INFO_SWITCH"},
	{FC_NODE_CONN_FAIL,				"FC_NODE_CONN_FAIL"},
	{FC_NODE_SOCKET_CHECK,			"FC_NODE_SOCKET_CHECK"},
	{FC_NODE_ALIVE_CHECK,			"FC_NODE_ALIVE_CHECK"},
	{FC_NODE_CONN_COMP,				"FC_NODE_CONN_COMP"},
	{FC_NODE_RANK_CHANGE,			"FC_NODE_RANK_CHANGE"}

};

static const wemsCommMessage_st arrCommMsg4[] =
{
	{FC_DMC_DB_UPDATE,				"FC_DMC_DB_UPDATE"},
	{FC_UPDATE_PDB,					"FC_UPDATE_PDB"},
	{FC_SEND_RTCONTROL_OUT,			"FC_SEND_RTCONTROL_OUT"},
	{FC_SEND_AIREALTIME_OUT,		"FC_SEND_AIREALTIME_OUT"},
	{FC_SEND_SCHEDULING_OUT,		"FC_SEND_SCHEDULING_OUT"},
	{FC_SEND_AISCHEDULING_OUT,		"FC_SEND_AISCHEDULING_OUT"},
	{FC_SEND_FORECAST_OUT,			"FC_SEND_FORECAST_OUT"},
	{FC_SEND_AIFORECAST_OUT,		"FC_SEND_AIFORECAST_OUT"},
	{FC_ALRAM_SYNC_PDB,				"FC_ALRAM_SYNC_PDB"},
	{FC_REQUEST_SYNC_PDB,			"FC_REQUEST_SYNC_PDB"},
	{FC_MW_TRIGGER_UPDATE,			"FC_MW_TRIGGER_UPDATE"},
	{FC_EMERGENCY_OPER_NOTIFY,		"FC_EMERGENCY_OPER_NOTIFY"},
	{FC_UPDATE_STAT_DATA,			"FC_UPDATE_STAT_DATA"},
	{FC_ARRANGE_RESOURCE,			"FC_ARRANGE_RESOURCE"},
	{FC_ASSEMMEA_DB_UPDATE,			"FC_ASSEMMEA_DB_UPDATE"}
};

static const wemsCommMessage_st arrCommMsg5[] =
{
	{FC_DAC_REGIST,					"FC_DAC_REGIST"},
	{FC_DAC_REGIST_RES_OK,			"FC_DAC_REGIST_RES_OK"},
	{FC_DAC_UNREGIST,				"FC_DAC_UNREGIST"},
	{FC_DAC_UNREGIST_RES_OK,		"FC_DAC_UNREGIST_RES_OK"},
	{FC_DAC_RUN_SCAN_ALL,			"FC_DAC_RUN_SCAN_ALL"},
	{FC_SCHD_BI,					"FC_SCHD_BI"},
	{FC_SCAN_BI,					"FC_SCAN_BI"},
	{FC_SCHD_AI,					"FC_SCHD_AI"},
	{FC_SCAN_AI,					"FC_SCAN_AI"},
	{FC_BI_DATA,					"FC_BI_DATA"},
	{FC_AI_DATA,					"FC_AI_DATA"},
	{FC_MEASUREMENT_DATA,			"FC_MEASUREMENT_DATA"},
	{FC_EVENT_GENERATION,			"FC_EVENT_GENERATION"},
	{FC_CTRL_BO,					"FC_CTRL_BO"},
	{FC_EMS_UPDATE_SEND,			"FC_EMS_UPDATE_SEND"},
	{FC_EMS_UI_CONTROL,				"FC_EMS_UI_CONTROL"},
	{FC_CTRL_AO,					"FC_CTRL_AO"},
	{FC_CTRL_OK,					"FC_CTRL_OK"},
	{FC_CTRL_NO,					"FC_CTRL_NO"},
	{FC_TEST_DAC_RUN_SCAN_ALL,		"FC_TEST_DAC_RUN_SCAN_ALL"}
};

static const wemsCommMessage_st arrCommMsg6[] =
{
	{FC_PDB_FILE_REQS,			"FC_PDB_FILE_REQS"},
	{FC_PDB_FILE_RESP,			"FC_PDB_FILE_RESP"},
	{FC_SHM_INFO_REQS,			"FC_SHM_INFO_REQS"},
	{FC_SHM_INFO_REPS,			"FC_SHM_INFO_REPS"},
	{FC_PID_SHM_INFO_REQS,		"FC_PID_SHM_INFO_REQS"},
	{FC_PID_SHM_INFO_REPS,		"FC_PID_SHM_INFO_REPS"}
};

static const wemsCommMessage_st arrCommMsg[] =
{
	{FC_PROC_REG_REQS,				"FC_PROC_REG_REQS"},
	{FC_PROC_TEST_REQS,				"FC_PROC_TEST_REQS"},
	{FC_PROC_START_REQS,			"FC_PROC_START_REQS"},
	{FC_PROC_STOP_REQS,				"FC_PROC_STOP_REQS"},
	{FC_PROC_KILL_REQS,				"FC_PROC_KILL_REQS"},
	{FC_PROC_SINGLE_START,			"FC_PROC_SINGLE_START"},
	{FC_PROC_SINGLE_STOP,			"FC_PROC_SINGLE_STOP"},
	{FC_PROC_SINGLE_KILL,			"FC_PROC_SINGLE_KILL"},
	{FC_PROC_LOG_CHANGE,			"FC_PROC_LOG_CHANGE"},
	{FC_PROC_WORK_CHECK,			"FC_PROC_WORK_CHECK"},
	{FC_PROC_SYSTEM_CHECK,			"FC_PROC_SYSTEM_CHECK"},
	{FC_PROC_RELOAD_CONFIG,			"FC_PROC_RELOAD_CONFIG"},
	{FC_NODE_INFO_REQS,				"FC_NODE_INFO_REQS"},
	{FC_NODE_INFO_RESP,				"FC_NODE_INFO_RESP"},
	{FC_NODE_INFO_UPDATE,			"FC_NODE_INFO_UPDATE"},
	{FC_NODE_INFO_TOTAL,			"FC_NODE_INFO_TOTAL"},
	{FC_NODE_INFO_CHANGE,			"FC_NODE_INFO_CHANGE"},
	{FC_NODE_INFO_SWITCH,			"FC_NODE_INFO_SWITCH"},
	{FC_NODE_CONN_FAIL,				"FC_NODE_CONN_FAIL"},
	{FC_NODE_SOCKET_CHECK,			"FC_NODE_SOCKET_CHECK"},
	{FC_NODE_ALIVE_CHECK,			"FC_NODE_ALIVE_CHECK"},
	{FC_NODE_CONN_COMP,				"FC_NODE_CONN_COMP"},
	{FC_PDB_FILE_REQS,				"FC_PDB_FILE_REQS"},
	{FC_PDB_FILE_RESP,				"FC_PDB_FILE_RESP"},
	{FC_SHM_INFO_REQS,				"FC_SHM_INFO_REQS"},
	{FC_SHM_INFO_REPS,				"FC_SHM_INFO_REPS"},
	{FC_PID_SHM_INFO_REQS,			"FC_PID_SHM_INFO_REQS"},
	{FC_PID_SHM_INFO_REPS,			"FC_PID_SHM_INFO_REPS"},
	{FC_WEMS_SCHD_ADD,				"FC_WEMS_SCHD_ADD"},
	{FC_WEMS_SCHD_DELETE,			"FC_WEMS_SCHD_DELETE"},
	{FC_WEMS_SCHD_QUERY,			"FC_WEMS_SCHD_QUERY"},
	{FC_WEMS_SCHD_STOP,				"FC_WEMS_SCHD_STOP"}
};

#endif