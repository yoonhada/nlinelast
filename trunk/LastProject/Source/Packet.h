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
	~CPacket();

	VOID	Copy( CHAR* pData, INT nSize );

	VOID	Rewind()	{ m_BuffPoint = 0; }
	CHAR*	GetBuff()	{ return m_Buff; }
	INT		GetLength()	{ return m_DataSize; }

	BOOL	ReadString( CHAR* pOut, WORD wMaxSize );	// wMaxSize : pOut의 최대크기
	BOOL	WriteString( CHAR* pData, WORD wSize );


	template<typename T>
	BOOL Read( T* a )
	{
		INT nSize = sizeof( T );
		INT remainedData = m_DataSize - m_BuffPoint;
		if( nSize > remainedData )
		{
			return FALSE;
		}

		memcpy( a, m_Buff + m_BuffPoint, sizeof( T ) );
		m_BuffPoint += sizeof( T );

		return TRUE;
	}


	template<typename T>
	BOOL Write( T a )
	{
		INT remained = PACKET_SIZE - m_BuffPoint;
		INT nSize = sizeof( T );
		if( remained < nSize )
		{
			return FALSE;
		}

		memcpy( m_Buff + m_BuffPoint, &a, nSize );
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