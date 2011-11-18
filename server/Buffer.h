#ifndef _CBUFFER_H_
#define _CBUFFER_H_


class CBuffer
{
public:
	CHAR m_Buffer[BUFFER_SIZE];
	WORD m_End;	// 다음기록 위치


public:
	CBuffer();
	virtual ~CBuffer();

	BOOL	Write( CHAR* pData, WORD nSize );
	VOID	WriteCommit( WORD nSize );
	BOOL	ReadCommit( WORD nSize );

	UINT	GetStoredSize()		{ return m_End; }
	CHAR*	GetWritePointer()	{ return m_Buffer + m_End; }
	UINT	GetEmptySize()		{ return BUFFER_SIZE - m_End; }

};


#endif