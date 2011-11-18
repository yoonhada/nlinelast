#ifndef _PACKET_H_
#define _PACKET_H_


class CPacket
{
public:
	CHAR	m_Buff[PACKET_SIZE];
	WORD	m_BuffPoint;
	WORD	m_DataSize;


public:
	CPacket();
	virtual ~CPacket();

	VOID	Copy( CHAR* pData, WORD nSize );

	VOID	Rewind()		{ m_BuffPoint = 0; }
	CHAR*	GetBuff()		{ return m_Buff; }
	WORD	GetLength()		{ return m_DataSize; }

	BOOL	ReadString( CHAR* pOut, WORD wMaxSize );
	BOOL	WriteString( CHAR* pData, WORD wSize );
	VOID	CalcSize();


	template<typename T>
	BOOL Write( T a )
	{
		WORD remained = PACKET_SIZE - m_BuffPoint;
		WORD nSize = sizeof( T );
		if( remained < nSize )
		{
			return FALSE;
		}

		memcpy( m_Buff + m_BuffPoint, &a, nSize );
		m_BuffPoint += nSize;
		m_DataSize += nSize;
		
		return TRUE;
	}

	template<typename T>
	BOOL Read( T* a )
	{
		WORD nSize = sizeof( T );
		WORD remainedData = m_DataSize - m_BuffPoint;
		if( nSize > remainedData )
		{
			return FALSE;
		}

		memcpy( a, m_Buff + m_BuffPoint, nSize );
		m_BuffPoint += nSize;

		return TRUE;
	}
};


#endif