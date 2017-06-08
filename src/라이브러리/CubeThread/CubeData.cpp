#include <string.h>
#include <stdlib.h>
#include "thread/CubeData.h"


CubeData::CubeData(void)
	:buffer_(NULL), bufsize_(0)
{
}


CubeData::CubeData(int buf_size)
	: buffer_(NULL), bufsize_(buf_size)
{
	allocate_size(buf_size);
}


CubeData::~CubeData(void)
{
	if(buffer_ != NULL)
	{
		free(buffer_);
		buffer_ = NULL;
	}
}

void*					CubeData::get_buffer( ) const
{
	return buffer_;
}

void					CubeData::set_buffer  ( const void* pBuf, unsigned int nSize )
{
	allocate_size(nSize);

	if(buffer_ != NULL)
		memcpy(buffer_, pBuf, nSize);

	return;
}

void					CubeData::set_swell_buffer  ( const void* pBuf, unsigned int nSize )
{
	if(buffer_ != NULL)
	{
		free(buffer_);
		buffer_ = NULL;
	}

	buffer_ = (char*)pBuf;
	bufsize_ = nSize;
}

size_t					CubeData::buffer_size ( )
{
	return bufsize_;
}



int		CubeData::allocate_size( size_t nSize )
{
	if(buffer_ != NULL)
	{
		free(buffer_);
		buffer_ = NULL;
	}

	if(nSize <= 0)
		return -1;

	buffer_ = (void *)malloc( nSize );
	memset(buffer_, 0x00, nSize);
	bufsize_ = nSize;

	return 0;
}


void CubeData::SetProcInfo(unsigned short usNodeCode, unsigned short usCopyNo, char* szProcName)
{
	this->usNodeCode_ = usNodeCode;
	this->usCopyNo_ = usCopyNo;
	strcpy(this->szProcName_, szProcName);
}