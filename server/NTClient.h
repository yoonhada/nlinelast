#ifndef _CNTCLIENT_H_
#define _CNTCLIENT_H_

#include "Packet.h"
#include "Buffer.h"


class CNTClient
{
public:
	SOCKET	m_Socket;
	WORD	m_Index;

	OVERLAPPED_PLUS m_OV_IN;
	OVERLAPPED_PLUS m_OV_OUT;

	CRITICAL_SECTION m_cs;

	CBuffer m_SendBuffer;
	CBuffer m_RecvBuffer;


public:
	CNTClient();
	~CNTClient();

	VOID Initialize();
	VOID Close();

	BOOL SendRest();
	BOOL PostRecv();

	BOOL Send( CPacket& pk );
	BOOL SendComplete( DWORD dwBytes );
};


#endif