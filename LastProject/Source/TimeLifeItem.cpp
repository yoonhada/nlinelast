/**
@file	TimeLifeItem.cpp
@date	2011/11/22
@author	yoonhada@gmail.com
@brief	������ Ŭ����
*/

#include "stdafx.h"
#include "Charactor.h"
#include "TimeLifeItem.h"


CTimeLifeItem::CTimeLifeItem()
: m_fLifeTime(1000)
{
	m_fLife = 0;
	m_bMonster = TRUE;
	CCharactor::AliveCheck(FALSE);
}

CTimeLifeItem::~CTimeLifeItem()
{
}

VOID CTimeLifeItem::Update()
{
	if ( CollisionAtk( ) )
	{
		D3DXMATRIXA16 mat;
		D3DXMatrixIdentity( &mat );
		BreakQube( mat );
		// �׿����� �ű��.....   0, 0, 0, 
//		CObjectManage::GetInstance()->Send_NetworkSendDestroyData( FALSE );
	}
}

VOID CTimeLifeItem::SetActive( BOOL a_bActive )
{
	//if ( a_bActive == TRUE )
	//{
	//	if (m_nLife <= 0)
	//	{
			AliveCheck( a_bActive );
	//		m_nLife = m_nLifeTime;
						
			UpdateByValue( D3DXVECTOR3( Get_CharaPos().x, 0.0f, Get_CharaPos().z ), 0.0f );
			UpdateOtherPlayer( TRUE );
	//	}
	//}
	//else
	//{
	//	CCharactor::TestBreakCubeAll();
	//	m_nLife = 0;
	//}
}

VOID CTimeLifeItem::Render()
{
	if ( AliveCheck() )
	{
		D3DXMatrixIdentity( &m_matMonster );
		CCharactor::Render();
	}
}
