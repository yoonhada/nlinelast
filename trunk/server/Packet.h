#ifndef _PACKET_H_
#define _PACKET_H_


class CPacket
{
public:
	CHAR	m_Buff[PACKET_SIZE];
	UINT	m_BuffPoint;
	UINT	m_DataSize;


public:
	CPacket();
	virtual ~CPacket();

	VOID	Copy( CHAR* pData, UINT nSize );

	VOID	Rewind()		{ m_BuffPoint = 0; }
	CHAR*	GetBuff()		{ return m_Buff; }
	UINT	GetLength()		{ return m_DataSize; }

	BOOL	ReadString( LPWSTR pOut, WORD wMaxSize );
	BOOL	WriteString( LPWSTR pData, WORD wSize );
	VOID	CalcSize();


	template<typename T>
	BOOL Write( T a )
	{
		UINT remained = PACKET_SIZE - m_BuffPoint;
		UINT nSize = sizeof( T );
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
		UINT nSize = sizeof( T );
		UINT remainedData = m_DataSize - m_BuffPoint;
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