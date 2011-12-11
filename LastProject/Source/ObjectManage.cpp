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
	m_pLobbyScene	= NULL;
	m_iClientNumber = 0;
	m_iMaxCharaNum	= 4;

	m_pCharactors = NULL;
	//m_ppVirtualCharactors = NULL;

	m_pClown = NULL;
	m_pPanda = NULL;
	m_pMonster = NULL;
	m_pFirstAidKit = NULL;
	m_pWall = NULL;

	m_pASEViewer = NULL;

	m_pLobbyScene = NULL;

	m_wTotalDestroyPart = 0;
}

HRESULT CObjectManage::Create( LPDIRECT3DDEVICE9 a_pD3dDevice )
{
	m_pD3dDevice = a_pD3dDevice;

	if( FAILED( D3DXCreateSprite( m_pD3dDevice, &m_pSprite ) ) )
	{
		MessageBox( NULL, L"CObjectManage::CreateSprite() failed", NULL, MB_OK );
		return E_FAIL;
	}

	LoadLoadingObject();
	LoadLobbyObject();
	LoadMainObject();

	return S_OK;
}	
	
HRESULT CObjectManage::LoadLoadingObject()
{
	m_pPanda = new CMonster;
	m_pPanda->Set_MonsterNumber( CGameEvent::PANDA );

	m_pBear = new CMonster;
	m_pBear->Set_MonsterNumber( CGameEvent::BEAR );

	// Monster
	m_pClown = new CMonster;
	m_pClown->Set_MonsterNumber( CGameEvent::CLOWN );

	m_pMonster = new CMonster*[3];
	m_pMonster[0] = m_pPanda;
	m_pMonster[1] = m_pBear;
	m_pMonster[2] = m_pClown;

	m_pFirstAidKit = new CTimeLifeItem[4];
	m_pWall = new CTimeLifeItem[3];

	return S_OK;
}

HRESULT CObjectManage::LoadLobbyObject()
{
	// 캐릭터 생성
	m_pCharactors = new CCharactor[ m_iMaxCharaNum ];
	for(INT Loop = 0; Loop < m_iMaxCharaNum; ++Loop )
	{
		m_pCharactors[Loop].Create( m_pD3dDevice );
		m_pCharactors[Loop].LoadKindChar( Loop );
	}
	//m_ppVirtualCharactors = new CCharactor * [m_iMaxCharaNum];

	//for ( int i = 0; i < m_iMaxCharaNum; ++i )
	//{
	//	m_ppVirtualCharactors[i] = &(m_pCharactors[i]);
	//}
	return S_OK;
}

HRESULT CObjectManage::LoadMainObject()
{
	// Map
	m_pASEViewer = new ASEViewer( m_pD3dDevice );

	return S_OK;
}

HRESULT CObjectManage::Release()
{
	SAFE_RELEASE( m_pSprite );

	SAFE_DELETE( m_pASEViewer );

	SAFE_DELETE_ARRAY( m_pCharactors );
	SAFE_DELETE_ARRAY( m_pMonster );
	SAFE_DELETE( m_pClown );
	SAFE_DELETE( m_pPanda );
	SAFE_DELETE( m_pBear );
	SAFE_DELETE_ARRAY( m_pFirstAidKit );
	SAFE_DELETE_ARRAY( m_pWall );

	return S_OK;
}

VOID CObjectManage::Set_PushBackNetworkSendTempVector( WORD a_wData )
{
	//CDebugConsole::GetInstance()->Messagef( L"PushBack Size : %d / Data : %d\n", m_NetworkSendTempVector.size(), a_wData );
	m_NetworkSendTempVector.push_back(a_wData);
}

VOID CObjectManage::Set_NetworkSendDestroyData( CHAR a_cDestroyPart, WORD a_wDestroyCount, D3DXVECTOR3& a_vDestroyDir)
{
	m_wDestroyPart[m_wTotalDestroyPart] = a_cDestroyPart;
	m_wDestroyCount[m_wTotalDestroyPart] = a_wDestroyCount;
	m_vDestroyDir = a_vDestroyDir;

	++m_wTotalDestroyPart;
}

VOID CObjectManage::Send_NetworkSendDestroyData( BOOL IsMonster, WORD wMonsterNumber )
{
	if( m_wTotalDestroyPart <= 0 )
		return;

	if( IsMonster == TRUE )	
	{
		CNetwork::GetInstance()->CS_UTOM_ATTACK( wMonsterNumber, m_vDestroyDir, m_wTotalDestroyPart, m_wDestroyPart, m_wDestroyCount, m_NetworkSendTempVector );
	}
	else if( IsMonster == FALSE )	
	{
		CNetwork::GetInstance()->CS_MTOU_ATTACK( m_vDestroyDir, m_wDestroyCount[0], m_NetworkSendTempVector );
	}
	else
	{
		CNetwork::GetInstance()->CS_EVENT_ATTACK( wMonsterNumber, m_vDestroyDir, m_wDestroyCount[0], m_NetworkSendTempVector );
	}

	m_wTotalDestroyPart = 0;
	m_NetworkSendTempVector.erase( m_NetworkSendTempVector.begin(), m_NetworkSendTempVector.end() );
	ZeroMemory( &m_wDestroyPart, sizeof(m_wDestroyPart) );
	ZeroMemory( &m_wDestroyCount, sizeof(m_wDestroyCount) );
	ZeroMemory( m_vDestroyDir, sizeof(m_vDestroyDir) );
}

//VOID CObjectManage::Set_Char(INT nSelect, INT nChar, BOOL bActive )
//{
//	//m_ppVirtualCharactors[nSelect] = &m_pCharactors[nChar];
//
//	if ( bActive )
//	{
//		m_pCharactors[nChar].Set_Active( TRUE );
//	}
//}

VOID CObjectManage::Set_CharTable( INT * nTable )
{
	for (INT i = 0; i < 4; ++i )
	{
		m_nCharTable[i] = nTable[i];

		if (m_nCharTable[i] != -1)
		{
			m_pCharactors[ m_nCharTable[i] ].Set_Active( TRUE );
		}
	}
}