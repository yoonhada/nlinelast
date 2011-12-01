/**
@file	ObjectManage.cpp
@date	2011/11/11
@author	백경훈
@brief	오브젝트 관리 ( 네트워크와 클래스간 사용 )
*/

#include "Stdafx.h"
#include "ObjectManage.h"

#include "Charactor.h"
#include "Monster.h"
#include "TimeLifeItem.h"
#include "ASEViewer.h"
#include "LobbyScene.h"

CObjectManage::CObjectManage()
{
	Clear();
}

CObjectManage::~CObjectManage()
{
	Release();
}

VOID CObjectManage::Clear()
{
	m_bHost			= FALSE;
	m_pCharactors	= NULL;
	m_pASEViewer	= NULL;
	m_pLobbyScene	= NULL;
	m_iClientNumber = 0;
	m_iMaxCharaNum	= 4;

	m_wTotalDestroyPart = 0;
}

HRESULT CObjectManage::Create( LPDIRECT3DDEVICE9 a_pD3dDevice )
{
	m_pD3dDevice = a_pD3dDevice;
	
	//캐릭터 생성
	m_pCharactors = new CCharactor[ m_iMaxCharaNum ];

	m_pMonster = new CMonster;

	m_pFirstAidKit = new CTimeLifeItem;

	m_pASEViewer = new ASEViewer( m_pD3dDevice );
	
	m_ppVirtualCharactors = new CCharactor * [m_iMaxCharaNum];

	for ( int i = 0; i < m_iMaxCharaNum; ++i )
	{
		m_ppVirtualCharactors[i] = &(m_pCharactors[i]);
	}

	return S_OK;
}

HRESULT CObjectManage::Release()
{
	SAFE_DELETE_ARRAY( m_pCharactors );
	SAFE_DELETE_ARRAY( m_ppVirtualCharactors );
	SAFE_DELETE( m_pMonster );
	SAFE_DELETE( m_pFirstAidKit );
	SAFE_DELETE( m_pASEViewer );

	return S_OK;
}

VOID CObjectManage::Set_PushBackNetworkSendTempVector( WORD a_wData )
{
	CDebugConsole::GetInstance()->Messagef( L"PushBack Size : %d / Data : %d\n", m_NetworkSendTempVector.size(), a_wData );
	m_NetworkSendTempVector.push_back(a_wData);
}

VOID CObjectManage::Set_NetworkSendDestroyData( CHAR a_cDestroyPart, WORD a_wDestroyCount, D3DXVECTOR3& a_vDestroyDir )
{
	m_wDestroyPart[m_wTotalDestroyPart] = a_cDestroyPart;
	m_wDestroyCount[m_wTotalDestroyPart] = a_wDestroyCount;
	m_vDestroyDir = a_vDestroyDir;

	++m_wTotalDestroyPart;
}

//VOID CObjectManage::Set_NetworkSendDestroyData( WORD a_wDestroyCount, D3DXVECTOR3& a_vDestroyDir )
//{
//	m_wDestroyPart[m_wTotalDestroyPart] = 0;
//	m_wDestroyCount[m_wTotalDestroyPart] = a_wDestroyCount;
//	m_vDestroyDir = a_vDestroyDir;
//
//	++m_wTotalDestroyPart;
//}

VOID CObjectManage::Send_NetworkSendDestroyData(BOOL _bIsCharter)
{
	if( m_wTotalDestroyPart <= 0 )
		return;

	if( _bIsCharter == FALSE)
	{
		CNetwork::GetInstance()->CS_UTOM_ATTACK( m_vDestroyDir, m_wTotalDestroyPart, m_wDestroyPart, m_wDestroyCount, m_NetworkSendTempVector );
		m_NetworkSendTempVector.clear();
		CDebugConsole::GetInstance()->Messagef( L"U-M m_NetworkSendTempVector Size : %d\n", m_NetworkSendTempVector.size() );
	}
	else
	{
		CNetwork::GetInstance()->CS_MTOU_ATTACK( m_vDestroyDir, m_wDestroyCount[0], m_NetworkSendTempVector );
		m_NetworkSendTempVector.clear();
		CDebugConsole::GetInstance()->Messagef( L"M-U m_NetworkSendTempVector Size : %d\n", m_NetworkSendTempVector.size() );
	}

	m_wTotalDestroyPart = 0;
	ZeroMemory( &m_wDestroyPart, sizeof(m_wDestroyPart) );
	ZeroMemory( &m_wDestroyCount, sizeof(m_wDestroyCount) );
	ZeroMemory( m_vDestroyDir, sizeof(m_vDestroyDir) );

}

VOID CObjectManage::Set_Char(INT nSelect, INT nChar, BOOL bActive )
{
	m_ppVirtualCharactors[nSelect] = &m_pCharactors[nChar];

	if ( bActive )
	{
		m_pCharactors[nChar].Set_Active( TRUE );
	}
}

