#pragma once

#include "sec/SecCommTypes.h"

#ifndef WELLS_Export
#ifdef WELLS_DLL_EXPORTS
#define WELLS_Export __declspec(dllexport)
#else
#define WELLS_Export __declspec(dllimport)
#endif
#endif

class WELLS_Export SecBaseConfig
{
public:
	SecBaseConfig(void);
	~SecBaseConfig(void);

	int					ConfigLoad();
	virtual int			ConfigLoad(char* szFileName);

	int		RealPort()		{ return real_port_; }
	int		ControlPort()	{ return control_port_; }
	int		MessagePort()	{ return message_port_; }

	char*	ServerAddr()	{ return server_addr_; }
	char*	ServerCode()	{ return server_code_; }

private:

	int					real_port_;
	int					control_port_;
	int					message_port_;

	char				server_addr_[MAX_ADDR_SZ];
	char				server_code_[16];
};

