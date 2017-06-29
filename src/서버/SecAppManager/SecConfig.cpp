#include "StdAfx.h"
#include "SecConfig.h"


SecConfig::SecConfig(void)
{
}


SecConfig::~SecConfig(void)
{
}

SecConfig* SecConfig::GetInstance()
{
	static SecConfig	_Instance;
	return &_Instance;
}