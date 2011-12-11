#ifndef _CPACKET_H_
#define _CPACKET_H_


class CPacket
{
public:
	CHAR	m_Buff[PACKET_SIZE];
	INT		m_BuffPoint;
	WORD	m_DataSize;


public:
	CPacket();
	virtual ~CPacket();

	VOID	Copy( CHAR* a_pData, INT a_nSize );

	VOID	Rewind()	{ m_BuffPoint = 0; }
	CHAR*	GetBuff()	{ return m_Buff; }
	INT		GetLength()	{ return m_DataSize; }

	BOOL	ReadString( LPWSTR a_pOut, WORD a_wMaxSize );	// wMaxSize : pOut의 최대크기
	BOOL	WriteString( LPWSTR a_pData, WORD a_wSize );
	VOID	CalcSize();


	template<typename T>
	BOOL Read( T* a_pDest )
	{
		INT nSize = sizeof( T );
		INT remainedData = m_DataSize - m_BuffPoint;
		if( nSize > remainedData )
		{
			return FALSE;
		}

		memcpy( a_pDest, m_Buff + m_BuffPoint, sizeof( T ) );
		m_BuffPoint += sizeof( T );

		return TRUE;
	}


	template<typename T>
	BOOL Write( T a_Src )
	{
		INT remained = PACKET_SIZE - m_BuffPoint;
		INT nSize = sizeof( T );
		if( remained < nSize )
		{
			return FALSE;
		}

		memcpy( m_Buff + m_BuffPoint, &a_Src, nSize );
		m_BuffPoint += nSize;
		m_DataSize += nSize;

		if( m_BuffPoint > 2 )
		{
			memcpy( m_Buff, &m_DataSize, sizeof( WORD ) );
		}

		return TRUE;
	}
};


#endif