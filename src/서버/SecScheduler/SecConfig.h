#pragma once
#include "sec/SecBaseConfig.h"

class SecConfig : public SecBaseConfig
{
public:
	SecConfig(void);
public:
	~SecConfig(void);

	static SecConfig* GetInstance();

private:
};

