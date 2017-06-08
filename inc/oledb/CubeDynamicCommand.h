#pragma once
#include <atldbcli.h>
#include <string>

#define		BLOB_MAXSIZE		0xFFFF

static const char *szNullString = "";
static const wchar_t *wszNullString = L"";

class CubeDynamicCommand : public ATL::CCommand<CDynamicAccessor, CRowset, CMultipleResults>
{
public:
	CubeDynamicCommand(const std::wstring& qry_string):
	  strQuery_(qry_string){}
	~CubeDynamicCommand(void){}

public:
	template<typename T> void GET_DATA_NULL_CHECK(int Index,T* value)
	{
		*value = (T)GetValue(Index);
		DBSTATUS status;
		GetStatus( Index, &status );
		DBTYPE type;
		GetColumnType(Index, &type);
		if( status == DBSTATUS_S_ISNULL )
		{
			if(type == DBTYPE_STR || type == DBTYPE_WSTR)
				*value = 0;
			else 
				**value = 0;
		}
	}
	template<> void GET_DATA_NULL_CHECK<DBTIMESTAMP*>(int Index,DBTIMESTAMP** value)
	{
		*value = (DBTIMESTAMP*)GetValue(Index);
		DBSTATUS status;
		GetStatus( Index, &status );
		DBTYPE type;
		GetColumnType(Index, &type);
		if( status == DBSTATUS_S_ISNULL )
		{
			*value=0;
		}
	}

	template<> void GET_DATA_NULL_CHECK<BYTE*>(int Index,BYTE** value)
	{
		DBSTATUS status;
		DBTYPE type;
		GetStatus( Index, &status );
		GetColumnType(Index, &type);


		if(type == DBTYPE_IUNKNOWN ) 
		{
			ULONG			pBufferSize(0);
			GetLength( Index, (DBLENGTH*)&pBufferSize );

			IStream*		pStream;
			pStream			= *(IStream**)_GetDataPtr( Index );

			BYTE		*pTBuffer = new BYTE[pBufferSize];
			BYTE		*pBuffer = new BYTE[BLOB_MAXSIZE];
			ULONG		nTSize(0), nRtSize(0);
			HRESULT     hr;

			memset( pTBuffer, 0, pBufferSize );
			do
			{
				memset( pBuffer, 0, BLOB_MAXSIZE );
				if( SUCCEEDED(hr = pStream->Read( pBuffer, BLOB_MAXSIZE, &nRtSize ) ) )
				{
					memcpy( pTBuffer+nTSize, pBuffer, nRtSize );
					nTSize += nRtSize;
				}
			} while ( SUCCEEDED(hr) && pBufferSize > nTSize );
			delete [] pBuffer;
			*value = pTBuffer;
		}
		else if(type == DBTYPE_UI1)
		{
			*value = (BYTE*)GetValue(Index);
			DBSTATUS status;
			GetStatus( Index, &status );
			DBTYPE type;
			GetColumnType(Index, &type);
			if( status == DBSTATUS_S_ISNULL )
			{
				**value = 0;
			}
		}
		else 
		{
			*value=0;
		} 
	}


	template <typename T1> 
	void GetColumnValues(T1* value1)
	{
		GET_DATA_NULL_CHECK(1, value1);
	}
	template <typename T1,typename T2> 
	void GetColumnValues(T1* value1,T2* value2)
	{
		GET_DATA_NULL_CHECK(1,  value1);
		GET_DATA_NULL_CHECK(2,  value2);

	}
	template <typename T1,typename T2, typename T3> 
	void GetColumnValues(T1* value1,T2* value2,T3* value3)
	{
		GET_DATA_NULL_CHECK(1, value1);
		GET_DATA_NULL_CHECK(2, value2);
		GET_DATA_NULL_CHECK(3, value3);
	}
	template <typename T1,typename T2, typename T3,typename T4> 
	void GetColumnValues(T1* value1,T2* value2,T3* value3,T4* value4)
	{
		GET_DATA_NULL_CHECK(1,  value1);
		GET_DATA_NULL_CHECK(2,  value2);
		GET_DATA_NULL_CHECK(3,  value3);
		GET_DATA_NULL_CHECK(4,  value4);
	}
	template <typename T1,typename T2, typename T3,typename T4,typename T5> 
	void GetColumnValues(T1* value1,T2* value2,T3* value3,T4* value4,T5* value5)
	{
		GET_DATA_NULL_CHECK(1, value1);
		GET_DATA_NULL_CHECK(2, value2);
		GET_DATA_NULL_CHECK(3,  value3);
		GET_DATA_NULL_CHECK(4,  value4);
		GET_DATA_NULL_CHECK(5,  value5);
	}
	template <typename T1,typename T2, typename T3,typename T4,typename T5,typename T6> 
	void GetColumnValues(T1* value1,T2* value2,T3* value3,T4* value4,T5* value5,
		T6* value6)
	{
		GET_DATA_NULL_CHECK(1,  value1);
		GET_DATA_NULL_CHECK(2,  value2);
		GET_DATA_NULL_CHECK(3,  value3);
		GET_DATA_NULL_CHECK(4,  value4);
		GET_DATA_NULL_CHECK(5,  value5);
		GET_DATA_NULL_CHECK(6,  value6);
	}
	template <typename T1,typename T2, typename T3,typename T4,typename T5,typename T6,typename T7> 
	void GetColumnValues(T1* value1,T2* value2,T3* value3,T4* value4,T5* value5,
		T6* value6,T7* value7)
	{
		GET_DATA_NULL_CHECK(1,  value1);
		GET_DATA_NULL_CHECK(2,  value2);
		GET_DATA_NULL_CHECK(3,  value3);
		GET_DATA_NULL_CHECK(4,  value4);
		GET_DATA_NULL_CHECK(5,  value5);
		GET_DATA_NULL_CHECK(6,  value6);
		GET_DATA_NULL_CHECK(7,  value7);
	}
	template <typename T1,typename T2, typename T3,typename T4,typename T5,typename T6,typename T7,typename T8> 
	void GetColumnValues(T1* value1,T2* value2,T3* value3,T4* value4,T5* value5,
		T6* value6,T7* value7,T8* value8)
	{
		GET_DATA_NULL_CHECK(1,  value1);
		GET_DATA_NULL_CHECK(2,  value2);
		GET_DATA_NULL_CHECK(3,  value3);
		GET_DATA_NULL_CHECK(4,  value4);
		GET_DATA_NULL_CHECK(5,  value5);
		GET_DATA_NULL_CHECK(6,  value6);
		GET_DATA_NULL_CHECK(7,  value7);
		GET_DATA_NULL_CHECK(8,  value8);

	}
	template <typename T1,typename T2, typename T3,typename T4,
		typename T5,typename T6,typename T7,typename T8,typename T9> 
		void GetColumnValues(T1* value1,T2* value2,T3* value3,T4* value4,T5* value5,
		T6* value6,T7* value7,T8* value8,T9* value9)
	{
		GET_DATA_NULL_CHECK(1,  value1);
		GET_DATA_NULL_CHECK(2,  value2);
		GET_DATA_NULL_CHECK(3,  value3);
		GET_DATA_NULL_CHECK(4,  value4);
		GET_DATA_NULL_CHECK(5,  value5);
		GET_DATA_NULL_CHECK(6,  value6);
		GET_DATA_NULL_CHECK(7,  value7);
		GET_DATA_NULL_CHECK(8,  value8);
		GET_DATA_NULL_CHECK(9,  value9);
	}
	template <typename T1,typename T2, typename T3,typename T4,
		typename T5,typename T6,typename T7,typename T8,typename T9,typename T10> 
		void GetColumnValues(T1* value1,T2* value2,T3* value3,T4* value4,T5* value5,
		T6* value6,T7* value7,T8* value8,T9* value9,T10* value10)
	{
		GET_DATA_NULL_CHECK(1,  value1);
		GET_DATA_NULL_CHECK(2,  value2);
		GET_DATA_NULL_CHECK(3,  value3);
		GET_DATA_NULL_CHECK(4,  value4);
		GET_DATA_NULL_CHECK(5,  value5);
		GET_DATA_NULL_CHECK(6,  value6);
		GET_DATA_NULL_CHECK(7,  value7);
		GET_DATA_NULL_CHECK(8,  value8);
		GET_DATA_NULL_CHECK(9,  value9);
		GET_DATA_NULL_CHECK(10,  value10);
	}

	template <typename T1,typename T2, typename T3,typename T4,
		typename T5,typename T6,typename T7,typename T8,typename T9,typename T10,typename T11> 
		void GetColumnValues(T1* value1,T2* value2,T3* value3,T4* value4,T5* value5,
		T6* value6,T7* value7,T8* value8,T9* value9,T10* value10,T11* value11)
	{
		GET_DATA_NULL_CHECK(1,  value1);
		GET_DATA_NULL_CHECK(2, value2);
		GET_DATA_NULL_CHECK(3,  value3);
		GET_DATA_NULL_CHECK(4,  value4);
		GET_DATA_NULL_CHECK(5,  value5);
		GET_DATA_NULL_CHECK(6,  value6);
		GET_DATA_NULL_CHECK(7,  value7);
		GET_DATA_NULL_CHECK(8,  value8);
		GET_DATA_NULL_CHECK(9,  value9);
		GET_DATA_NULL_CHECK(10,  value10);
		GET_DATA_NULL_CHECK(11,  value11);
	}

	template <typename T1,typename T2, typename T3,typename T4,
		typename T5,typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,typename T12> 
		void GetColumnValues(T1* value1,T2* value2,T3* value3,T4* value4,T5* value5,
		T6* value6,T7* value7,T8* value8,T9* value9,T10* value10,T11* value11,T12* value12)
	{
		GET_DATA_NULL_CHECK(1,  value1);
		GET_DATA_NULL_CHECK(2, value2);
		GET_DATA_NULL_CHECK(3,  value3);
		GET_DATA_NULL_CHECK(4,  value4);
		GET_DATA_NULL_CHECK(5,  value5);
		GET_DATA_NULL_CHECK(6,  value6);
		GET_DATA_NULL_CHECK(7,  value7);
		GET_DATA_NULL_CHECK(8,  value8);
		GET_DATA_NULL_CHECK(9,  value9);
		GET_DATA_NULL_CHECK(10,  value10);
		GET_DATA_NULL_CHECK(11,  value11);
		GET_DATA_NULL_CHECK(12,  value12);
	}
	template <typename T1,typename T2, typename T3,typename T4,
		typename T5,typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,typename T12,
		typename T13> 
		void GetColumnValues(T1* value1,T2* value2,T3* value3,T4* value4,T5* value5,
		T6* value6,T7* value7,T8* value8,T9* value9,T10* value10,T11* value11,T12* value12,T13* value13)
	{
		GET_DATA_NULL_CHECK(1,  value1);
		GET_DATA_NULL_CHECK(2, value2);
		GET_DATA_NULL_CHECK(3,  value3);
		GET_DATA_NULL_CHECK(4,  value4);
		GET_DATA_NULL_CHECK(5,  value5);
		GET_DATA_NULL_CHECK(6,  value6);
		GET_DATA_NULL_CHECK(7,  value7);
		GET_DATA_NULL_CHECK(8,  value8);
		GET_DATA_NULL_CHECK(9,  value9);
		GET_DATA_NULL_CHECK(10,  value10);
		GET_DATA_NULL_CHECK(11,  value11);
		GET_DATA_NULL_CHECK(12,  value12);
		GET_DATA_NULL_CHECK(13,  value13);
	}
	template <typename T1,typename T2, typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10,
		typename T11,typename T12,typename T13,typename T14> 
		void GetColumnValues(T1* value1,T2* value2,T3* value3,T4* value4,T5* value5,T6* value6,T7* value7,T8* value8,T9* value9,T10* value10,
		T11* value11,T12* value12,T13* value13,T14* value14)
	{
		GET_DATA_NULL_CHECK(1,  value1);
		GET_DATA_NULL_CHECK(2, value2);
		GET_DATA_NULL_CHECK(3,  value3);
		GET_DATA_NULL_CHECK(4,  value4);
		GET_DATA_NULL_CHECK(5,  value5);
		GET_DATA_NULL_CHECK(6,  value6);
		GET_DATA_NULL_CHECK(7,  value7);
		GET_DATA_NULL_CHECK(8,  value8);
		GET_DATA_NULL_CHECK(9,  value9);
		GET_DATA_NULL_CHECK(10,  value10);
		GET_DATA_NULL_CHECK(11,  value11);
		GET_DATA_NULL_CHECK(12,  value12);
		GET_DATA_NULL_CHECK(13,  value13);
		GET_DATA_NULL_CHECK(14,  value14);
	}
	template <typename T1,typename T2, typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10,
		typename T11,typename T12,typename T13,typename T14,typename T15> 
		void GetColumnValues(T1* value1,T2* value2,T3* value3,T4* value4,T5* value5,T6* value6,T7* value7,T8* value8,T9* value9,T10* value10,
		T11* value11,T12* value12,T13* value13,T14* value14,T15* value15)
	{
		GET_DATA_NULL_CHECK(1,  value1);
		GET_DATA_NULL_CHECK(2, value2);
		GET_DATA_NULL_CHECK(3,  value3);
		GET_DATA_NULL_CHECK(4,  value4);
		GET_DATA_NULL_CHECK(5,  value5);
		GET_DATA_NULL_CHECK(6,  value6);
		GET_DATA_NULL_CHECK(7,  value7);
		GET_DATA_NULL_CHECK(8,  value8);
		GET_DATA_NULL_CHECK(9,  value9);
		GET_DATA_NULL_CHECK(10,  value10);
		GET_DATA_NULL_CHECK(11,  value11);
		GET_DATA_NULL_CHECK(12,  value12);
		GET_DATA_NULL_CHECK(13,  value13);
		GET_DATA_NULL_CHECK(14,  value14);
		GET_DATA_NULL_CHECK(15,  value15);
	}
	template <typename T1,typename T2, typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10,
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16> 
		void GetColumnValues(T1* value1,T2* value2,T3* value3,T4* value4,T5* value5,T6* value6,T7* value7,T8* value8,T9* value9,T10* value10,
		T11* value11,T12* value12,T13* value13,T14* value14,T15* value15,T16* value16)
	{
		GET_DATA_NULL_CHECK(1,  value1);
		GET_DATA_NULL_CHECK(2, value2);
		GET_DATA_NULL_CHECK(3,  value3);
		GET_DATA_NULL_CHECK(4,  value4);
		GET_DATA_NULL_CHECK(5,  value5);
		GET_DATA_NULL_CHECK(6,  value6);
		GET_DATA_NULL_CHECK(7,  value7);
		GET_DATA_NULL_CHECK(8,  value8);
		GET_DATA_NULL_CHECK(9,  value9);
		GET_DATA_NULL_CHECK(10,  value10);
		GET_DATA_NULL_CHECK(11,  value11);
		GET_DATA_NULL_CHECK(12,  value12);
		GET_DATA_NULL_CHECK(13,  value13);
		GET_DATA_NULL_CHECK(14,  value14);
		GET_DATA_NULL_CHECK(15,  value15);
		GET_DATA_NULL_CHECK(16,  value16);
	}
	template <typename T1,typename T2, typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10,
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17> 
		void GetColumnValues(T1* value1,T2* value2,T3* value3,T4* value4,T5* value5,T6* value6,T7* value7,T8* value8,T9* value9,T10* value10,
		T11* value11,T12* value12,T13* value13,T14* value14,T15* value15,T16* value16,T17* value17)
	{
		GET_DATA_NULL_CHECK(1,  value1);
		GET_DATA_NULL_CHECK(2, value2);
		GET_DATA_NULL_CHECK(3,  value3);
		GET_DATA_NULL_CHECK(4,  value4);
		GET_DATA_NULL_CHECK(5,  value5);
		GET_DATA_NULL_CHECK(6,  value6);
		GET_DATA_NULL_CHECK(7,  value7);
		GET_DATA_NULL_CHECK(8,  value8);
		GET_DATA_NULL_CHECK(9,  value9);
		GET_DATA_NULL_CHECK(10,  value10);
		GET_DATA_NULL_CHECK(11,  value11);
		GET_DATA_NULL_CHECK(12,  value12);
		GET_DATA_NULL_CHECK(13,  value13);
		GET_DATA_NULL_CHECK(14,  value14);
		GET_DATA_NULL_CHECK(15,  value15);
		GET_DATA_NULL_CHECK(16,  value16);
		GET_DATA_NULL_CHECK(17,  value17);
	}
	template <typename T1,typename T2, typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10,
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18> 
		void GetColumnValues(T1* value1,T2* value2,T3* value3,T4* value4,T5* value5,T6* value6,T7* value7,T8* value8,T9* value9,T10* value10,
		T11* value11,T12* value12,T13* value13,T14* value14,T15* value15,T16* value16,T17* value17,T18* value18)
	{
		GET_DATA_NULL_CHECK(1,  value1);
		GET_DATA_NULL_CHECK(2,	value2);
		GET_DATA_NULL_CHECK(3,  value3);
		GET_DATA_NULL_CHECK(4,  value4);
		GET_DATA_NULL_CHECK(5,  value5);
		GET_DATA_NULL_CHECK(6,  value6);
		GET_DATA_NULL_CHECK(7,  value7);
		GET_DATA_NULL_CHECK(8,  value8);
		GET_DATA_NULL_CHECK(9,  value9);
		GET_DATA_NULL_CHECK(10,  value10);
		GET_DATA_NULL_CHECK(11,  value11);
		GET_DATA_NULL_CHECK(12,  value12);
		GET_DATA_NULL_CHECK(13,  value13);
		GET_DATA_NULL_CHECK(14,  value14);
		GET_DATA_NULL_CHECK(15,  value15);
		GET_DATA_NULL_CHECK(16,  value16);
		GET_DATA_NULL_CHECK(17,  value17);
		GET_DATA_NULL_CHECK(18,  value18);
	}
	template <typename T1,typename T2, typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10,
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19> 
		void GetColumnValues(T1* value1,T2* value2,T3* value3,T4* value4,T5* value5,T6* value6,T7* value7,T8* value8,T9* value9,T10* value10,
		T11* value11,T12* value12,T13* value13,T14* value14,T15* value15,T16* value16,T17* value17,T18* value18,T19* value19)
	{
		GET_DATA_NULL_CHECK(1,  value1);
		GET_DATA_NULL_CHECK(2, value2);
		GET_DATA_NULL_CHECK(3,  value3);
		GET_DATA_NULL_CHECK(4,  value4);
		GET_DATA_NULL_CHECK(5,  value5);
		GET_DATA_NULL_CHECK(6,  value6);
		GET_DATA_NULL_CHECK(7,  value7);
		GET_DATA_NULL_CHECK(8,  value8);
		GET_DATA_NULL_CHECK(9,  value9);
		GET_DATA_NULL_CHECK(10,  value10);
		GET_DATA_NULL_CHECK(11,  value11);
		GET_DATA_NULL_CHECK(12,  value12);
		GET_DATA_NULL_CHECK(13,  value13);
		GET_DATA_NULL_CHECK(14,  value14);
		GET_DATA_NULL_CHECK(15,  value15);
		GET_DATA_NULL_CHECK(16,  value16);
		GET_DATA_NULL_CHECK(17,  value17);
		GET_DATA_NULL_CHECK(18,  value18);
		GET_DATA_NULL_CHECK(19,  value19);
	}
	template <typename T1,typename T2, typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10,
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20> 
		void GetColumnValues(T1* value1,T2* value2,T3* value3,T4* value4,T5* value5,T6* value6,T7* value7,T8* value8,T9* value9,T10* value10,
		T11* value11,T12* value12,T13* value13,T14* value14,T15* value15,T16* value16,T17* value17,T18* value18,T19* value19,T20* value20)
	{
		GET_DATA_NULL_CHECK(1,  value1);
		GET_DATA_NULL_CHECK(2, value2);
		GET_DATA_NULL_CHECK(3,  value3);
		GET_DATA_NULL_CHECK(4,  value4);
		GET_DATA_NULL_CHECK(5,  value5);
		GET_DATA_NULL_CHECK(6,  value6);
		GET_DATA_NULL_CHECK(7,  value7);
		GET_DATA_NULL_CHECK(8,  value8);
		GET_DATA_NULL_CHECK(9,  value9);
		GET_DATA_NULL_CHECK(10,  value10);
		GET_DATA_NULL_CHECK(11,  value11);
		GET_DATA_NULL_CHECK(12,  value12);
		GET_DATA_NULL_CHECK(13,  value13);
		GET_DATA_NULL_CHECK(14,  value14);
		GET_DATA_NULL_CHECK(15,  value15);
		GET_DATA_NULL_CHECK(16,  value16);
		GET_DATA_NULL_CHECK(17,  value17);
		GET_DATA_NULL_CHECK(18,  value18);
		GET_DATA_NULL_CHECK(19,  value19);
		GET_DATA_NULL_CHECK(20,  value20);
	}

	template <typename T1,typename T2, typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10,
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20,typename T21> 
		void GetColumnValues(T1* value1,T2* value2,T3* value3,T4* value4,T5* value5,T6* value6,T7* value7,T8* value8,T9* value9,T10* value10,
		T11* value11,T12* value12,T13* value13,T14* value14,T15* value15,T16* value16,T17* value17,T18* value18,T19* value19,T20* value20, T21* value21)
	{
		GET_DATA_NULL_CHECK(1,  value1);
		GET_DATA_NULL_CHECK(2, value2);
		GET_DATA_NULL_CHECK(3,  value3);
		GET_DATA_NULL_CHECK(4,  value4);
		GET_DATA_NULL_CHECK(5,  value5);
		GET_DATA_NULL_CHECK(6,  value6);
		GET_DATA_NULL_CHECK(7,  value7);
		GET_DATA_NULL_CHECK(8,  value8);
		GET_DATA_NULL_CHECK(9,  value9);
		GET_DATA_NULL_CHECK(10,  value10);
		GET_DATA_NULL_CHECK(11,  value11);
		GET_DATA_NULL_CHECK(12,  value12);
		GET_DATA_NULL_CHECK(13,  value13);
		GET_DATA_NULL_CHECK(14,  value14);
		GET_DATA_NULL_CHECK(15,  value15);
		GET_DATA_NULL_CHECK(16,  value16);
		GET_DATA_NULL_CHECK(17,  value17);
		GET_DATA_NULL_CHECK(18,  value18);
		GET_DATA_NULL_CHECK(19,  value19);
		GET_DATA_NULL_CHECK(20,  value20);
		GET_DATA_NULL_CHECK(21,  value21);
	}

	template <typename T1,typename T2, typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10,
		typename T11,typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,typename T18,typename T19,typename T20,typename T21,typename T22> 
		void GetColumnValues(T1* value1,T2* value2,T3* value3,T4* value4,T5* value5,T6* value6,T7* value7,T8* value8,T9* value9,T10* value10,
		T11* value11,T12* value12,T13* value13,T14* value14,T15* value15,T16* value16,T17* value17,T18* value18,T19* value19,T20* value20, T21* value21, T22* value22)
	{
		GET_DATA_NULL_CHECK(1,  value1);
		GET_DATA_NULL_CHECK(2, value2);
		GET_DATA_NULL_CHECK(3,  value3);
		GET_DATA_NULL_CHECK(4,  value4);
		GET_DATA_NULL_CHECK(5,  value5);
		GET_DATA_NULL_CHECK(6,  value6);
		GET_DATA_NULL_CHECK(7,  value7);
		GET_DATA_NULL_CHECK(8,  value8);
		GET_DATA_NULL_CHECK(9,  value9);
		GET_DATA_NULL_CHECK(10,  value10);
		GET_DATA_NULL_CHECK(11,  value11);
		GET_DATA_NULL_CHECK(12,  value12);
		GET_DATA_NULL_CHECK(13,  value13);
		GET_DATA_NULL_CHECK(14,  value14);
		GET_DATA_NULL_CHECK(15,  value15);
		GET_DATA_NULL_CHECK(16,  value16);
		GET_DATA_NULL_CHECK(17,  value17);
		GET_DATA_NULL_CHECK(18,  value18);
		GET_DATA_NULL_CHECK(19,  value19);
		GET_DATA_NULL_CHECK(20,  value20);
		GET_DATA_NULL_CHECK(21,  value21);
		GET_DATA_NULL_CHECK(22,  value22);
	}

	unsigned long	GetCount()
	{
		if ( m_spRowset == 0 )
			return 0 ;

		unsigned long nCount;

		((CRowset *)this)->GetApproximatePosition(NULL, NULL, (DBCOUNTITEM*)&nCount);
		//HRESULT hr = GetApproximatePosition(NULL, NULL, (DBCOUNTITEM*)&nCount);

		return nCount;
	}
	bool	IsNull(long nColumn)
	{
		DBSTATUS status;
		GetStatus( nColumn, &status );
		if ( status == DBSTATUS_S_ISNULL )
			return true;
		return false;
	}


private:
	std::wstring		strQuery_;
};

