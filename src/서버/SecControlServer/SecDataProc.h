#pragma once

class SecDataProc
{
public:
	SecDataProc(void);
	~SecDataProc(void);

	static SecDataProc* GetInstance();

	int PacketDataProc(wemsGPN_st* stSrcProc, wemsGPN_st* stDestProc, wemsDataPacket_st* pRcvData);
};

