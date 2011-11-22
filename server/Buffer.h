#ifndef _CBUFFER_H_
#define _CBUFFER_H_


class CBuffer
{
public:
	CHAR m_Buffer[BUFFER_SIZE];
	UINT m_End;	// ������� ��ġ


public:
	CBuffer();
	virtual ~CBuffer();

	BOOL	Write( CHAR* pData, UINT nSize );
	VOID	WriteCommit( UINT nSize );
	BOOL	ReadCommit( UINT nSize );

	UINT	GetStoredSize()		{ return m_End; }
	CHAR*	GetWritePointer()	{ return m_Buffer + m_End; }
	UINT	GetEmptySize()		{ return BUFFER_SIZE - m_End; }

};


#endif