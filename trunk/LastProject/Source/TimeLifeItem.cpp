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
: m_fLifeTime(1000)
{
	m_fLife = 0;
	CCharactor::AliveCheck(FALSE);
}

CTimeLifeItem::~CTimeLifeItem()
{
}

VOID CTimeLifeItem::Update()
{
	//if ( m_nLife > m_nLifeTime )
	//{
	//	m_nLife += CFrequency::GetInstance()->getFrametime();
	//}
	//else if ( m_nLife > -m_nLifeTime )
	//{

	//}
	//else if ( m_nLife > -m_nLifeTime )
	//{
	//	m_nLife--;
	//}
	//else
	//{
	//	CCharactor::AliveCheck( FALSE );
	//}
}

VOID CTimeLifeItem::SetActive( BOOL a_bActive )
{
	//if ( a_bActive == TRUE )
	//{
	//	if (m_nLife <= 0)
	//	{
			AliveCheck( a_bActive );
	//		m_nLife = m_nLifeTime;

			FLOAT x = FastRand2() * 500.0f - 250.0f;
			FLOAT z = FastRand2() * 500.0f - 250.0f;

	//		
			UpdateByValue( D3DXVECTOR3( x, 0, z ), 0.0f );
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
		CCharactor::Render();
	}
}