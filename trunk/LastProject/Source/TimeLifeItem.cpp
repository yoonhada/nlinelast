/**
@file	TimeLifeItem.cpp
@date	2011/11/22
@author	yoonhada@gmail.com
@brief	아이템 클래스
*/

#include "stdafx.h"
#include "Charactor.h"
#include "TimeLifeItem.h"


CTimeLifeItem::CTimeLifeItem()
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
		CSound::GetInstance()->PlayEffect( CSound::EFFECT_HIT );

		if ( m_iMonsterNumber & 0x00F0 )
		{
			D3DXMATRIXA16 mat;
			D3DXMatrixIdentity( &mat );
			BreakQube( mat );

			CGameEvent::GetInstance()->Set_PlayerIndex( CObjectManage::GetInstance()->Get_CharTable( CObjectManage::GetInstance()->Get_ClientNumber() ) );
			CGameEvent::GetInstance()->Set_MonsterIndex( m_iMonsterNumber );
		}
		else if ( m_iMonsterNumber & 0xFF00 )
		{
			m_fLife = 3.0f;
			BreakCubeAll();
		}
	}
	if ( m_fLife > 0.0f )
	{
		m_fLife -= CFrequency::GetInstance()->getFrametime();
		if ( m_fLife < 0.0f )
		{
			AliveCheck( FALSE );
			CGameEvent::GetInstance()->Set_MonsterIndex( m_iMonsterNumber );
			CGameEvent::GetInstance()->AddEvent( CGameEvent::GAME_HEALING, 0.001f );
		}
	}
}

VOID CTimeLifeItem::SetActive( BOOL a_bActive )
{
	AliveCheck( a_bActive );

	UpdateByValue( D3DXVECTOR3( Get_CharaPos().x, 0.0f, Get_CharaPos().z ), 0.0f );
	UpdateOtherPlayer( TRUE );
}

VOID CTimeLifeItem::Render()
{
	if ( AliveCheck() )
	{
		D3DXMatrixIdentity( &m_matMonster );
		CCharactor::Render();
	}
}
