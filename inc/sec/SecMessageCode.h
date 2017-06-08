#ifndef __SEC_MESSAGE_CODE_H__
#define __SEC_MESSAGE_CODE_H__

/** ���μ��� �޽��� �ڵ� */
#define		FC_PROC_REG_REQS		(0x2001)		/*	���μ��� ��� ��û	*/
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

/** ��� �޽��� �ڵ� */
#define		FC_NODE_INFO_REQS		(0x3001)		//< ��� ���� ��û -> Ÿ���(ESSC, PMS.B) --> PMS.A || ESSC.B -> ESSC.A
#define		FC_NODE_INFO_RESP		(0x3002)		//< ��� ���� ���� -> PMS.A --> Ÿ���(ESSC, PMS.B) || ESSC.A -> ESSC.B
#define		FC_NODE_INFO_UPDATE		(0x3003)		//< �ڽ��� ��� ���� ����(������Ʈ��) -> Ÿ���(ESSC, PMS.B) --> PMS.A || ESSC.B -> ESSC.A
#define		FC_NODE_INFO_TOTAL		(0x3004)		//< ��ü ��� ���� ����(������Ʈ��) - PMS.A --> ��ü ����� ���
#define		FC_NODE_INFO_CHANGE		(0x3005)
#define		FC_NODE_INFO_SWITCH		(0x3006)
#define		FC_NODE_CONN_FAIL		(0x3007)
#define		FC_NODE_SOCKET_CHECK	(0x3008)
#define		FC_NODE_ALIVE_CHECK		(0x3009)
#define		FC_NODE_CONN_COMP		(0x300a)
#define		FC_NODE_RANK_CHANGE		(0x300b)


#define		FC_DMC_DB_UPDATE				0x4101		// DMC Pdb --> RDB (DMC ������ RDB �� �ݿ�)
#define		FC_UPDATE_PDB					0x4201		// wemsemsrcv --> DA's wemsrtpd (����EMS ������ Algorithm �� ������� pdb �� DA ���� ����)
#define		FC_SEND_RTCONTROL_OUT			0x4202		// DA wemsapmd --> DA wemsrtpd
#define		FC_SEND_AIREALTIME_OUT			0x4203		// DA wemsrtpd  --> UA wemsrtpd --> UA wemsemssnd --> U EMS
#define		FC_SEND_SCHEDULING_OUT			0x4204		// DA wemsapmd --> DA wemsrtpd
#define		FC_SEND_AISCHEDULING_OUT		0x4205		// UA wemsappmd --> UA wemsrtpd --> UA wemsemssnd --> U EMS
#define		FC_SEND_FORECAST_OUT			0x4206		// DA wemsapmd --> DA wemsrtpd
#define		FC_SEND_AIFORECAST_OUT			0x4207		// UA wemsappmd --> UA wemsrtpd --> UA wemsemssnd --> U EMS
#define		FC_ALRAM_SYNC_PDB				0x4208		// DA �� wemsrtpd �� ���� UA �� wemsrtpd ���� pdb (����� dmc ��) ����ȭ ����� ������� �˶�.
#define		FC_REQUEST_SYNC_PDB				0x4209		// DA �� wemsrtpd �� UA �� wemsrtpd ���� pdb �� �ָ鼭 ����ȭ �϶�� ���. (wemsschmd ���� FC_ALRAM_SYNC_PDB �� ���� �� ���� ����ȭ�� pdb �� ������ ���)
#define		FC_MW_TRIGGER_UPDATE			0x4210		// 30�ʸ��� update
#define		FC_EMERGENCY_OPER_NOTIFY		0x4211		// ������ ���Խ� DA �� rtinput.reserved5 �� 1 �� ����
#define		FC_UPDATE_STAT_DATA				0x4212		// ��赥���͸� �����϶�� ��û
#define		FC_ARRANGE_RESOURCE				0x4213		// ���ҽ��� �����϶�� ��û
#define		FC_ASSEMMEA_DB_UPDATE			0x4214		// ASSEM MEASURE PDB --> RDB(


#define		FC_DAC_REGIST					(0x5001)		// Regist DAC per Device to DAC Table
#define		FC_DAC_REGIST_RES_OK			(0x5002)		// ok result response of FC_DAC_REGIST
#define		FC_DAC_UNREGIST					(0x5003)		// UnRegist DAC per Device to DAC Table
#define		FC_DAC_UNREGIST_RES_OK			(0x5004)		// ok result response of FC_DAC_UNREGIST
#define		FC_DAC_RUN_SCAN_ALL				(0x5005)		// DAC �鿡�� Scan �� ���������� ����� ������
#define		FC_SCHD_BI						(0x5301)		// Binary Input Data      (read measure ��û)
#define		FC_SCAN_BI						(0x5302)		// Binary Input Scan Data (read measure ��û)
#define		FC_SCHD_AI						(0x5401)		// Analog Input Data      (read measure ��û)
#define		FC_SCAN_AI						(0x5402)		// Analog Input Scan Data (read measure ��û)
#define		FC_BI_DATA						(0x5501)		// measure read ��������� ����
#define		FC_AI_DATA						(0x5601)		// measure read ��������� ����
#define		FC_MEASUREMENT_DATA				(0x5602)		// measure �����͸� DB �� update �϶�� ��û
#define		FC_EVENT_GENERATION				(0X5603)		// ���� ���μ������� wemsrtpd ���� Event �� �߻��ߴٰ� �˸�. (wemsrtpd �� wemsrdbpd ���� bypass ��)
#define		FC_CTRL_BO						(0x5701)		// ������ (BO/DO)
#define		FC_EMS_UPDATE_SEND				(0X5702)		// EMS ���� ���������͸� ������ �ð��� �Ǿ��ٴ� �˶� �޽���
#define		FC_EMS_UI_CONTROL				(0X5703)		// EMS �� UI Control ������ (exist in rdb) �� ����A ���� �����ϴ� �޽���
#define		FC_CTRL_AO						(0x5704)		// ������ (AO)
#define     FC_CTRL_OK						(0x5705)
#define     FC_CTRL_NO						(0x5706)
// �׽�Ʈ�� Function Codes <<����>>
#define		FC_TEST_DAC_RUN_SCAN_ALL		(0x5999)		// DAC �鿡�� Scan �� ���������� ����� ������ (���� ������ �ƴϰ�, ������ �����͸� ��ġ ������ ��ó�� �����Ͽ� �÷���������)



#define		FC_PDB_FILE_REQS				(0x6001)			// PDB ���� ��û	PDBID,					wemssysmd -> wemssysmd => ������ �ε��Ͽ� ����.
#define		FC_PDB_FILE_RESP				(0x6002)			// PDB ���� ����    PDB ���� ����			wemssysmd -> wemssysmd => ���ŵ� ���� ���� ����
#define		FC_SHM_INFO_REQS				(0x6003)			// PDB(SHM) ID�� ���� �����޸� ��û
#define		FC_SHM_INFO_REPS				(0x6004)			// PDB(SHM) ID�� ���� �����޸� ��û�� ���� ����
#define		FC_PID_SHM_INFO_REQS			(0x6005)			// PDB(SHM) ID + PID �� ���� �����޸� ��û
#define		FC_PID_SHM_INFO_REPS			(0x6006)			// PDB(SHM) ID + PID �� ���� �����޸� ��û�� ���� ����


/** ������ �޽��� �ڵ� */
#define     FC_WEMS_SCHD_ADD        (0x1201)        /*  �����층 ������ �߰�            PROC SERVER SCHMD   */
#define     FC_WEMS_SCHD_DELETE     (0x1202)        /*  �����층 ������ ����            PROC SERVER SCHMD   */
#define     FC_WEMS_SCHD_QUERY      (0x1203)        /*  �����층 ������ ��û            PROC SERVER SCHMD   */
#define		FC_WEMS_SCHD_STOP		(0x1204)		/*  �����층 ����										*/

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