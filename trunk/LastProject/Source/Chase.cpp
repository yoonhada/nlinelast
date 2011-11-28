#include "stdafx.h"

#include "Chase.h"
#include "Seek.h"

#include "Frequency.h"
#include "Charactor.h"
#include "Monster.h"


Chase* Chase::GetInstance()
{
	static Chase Instance;

	return &Instance;
}


VOID Chase::Enter( CMonster* a_pMonster )
{
	m_pCurrentPath = a_pMonster->Get_Path();
	m_vPos = a_pMonster->Get_Pos();
	m_iCurrentX = INT( m_vPos.x + m_pMapInfo->vecEnd.x ) / m_pMapInfo->fTileSize;
	m_iCurrentZ = INT( -( m_vPos.z - m_pMapInfo->vecEnd.z ) ) / m_pMapInfo->fTileSize;
	a_pMonster->Set_Angle( D3DXToRadian( GetDegree() ) );
}


VOID Chase::Execute( CMonster* a_pMonster )
{
	static FLOAT t = 0.0f;
	t += CFrequency::GetInstance()->getFrametime();

	if( t >= 0.25f )
	{
		t = 0.0f;

		m_iCurrentX = m_pCurrentPath->x;
		m_iCurrentZ = m_pCurrentPath->y;
		
		m_pCurrentPath = m_pCurrentPath->next;
		if( m_pCurrentPath->next == NULL )
		{
			a_pMonster->GetFSM()->ChangeState( Seek::GetInstance() );
		}
		else
		{
			// 다음 위치로 몬스터 각을 맞춘다.
			FLOAT fDegree = GetDegree();
			a_pMonster->Set_Angle( D3DXToRadian( fDegree - 90.0f ) );

			// 다음 Path 설정
			m_vPos = a_pMonster->Get_Pos();
		}
	}
	else
	{
		D3DXVECTOR3 Pos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		D3DXVECTOR3 NextPos = D3DXVECTOR3( m_pMapInfo->vecStart.x + m_pMapInfo->fTileSize * m_pCurrentPath->x + m_pMapInfo->fTileSize / 2,
										   0.0f,
										   m_pMapInfo->vecEnd.z - m_pMapInfo->fTileSize * m_pCurrentPath->y - m_pMapInfo->fTileSize / 2 );

		D3DXVec3Lerp( &Pos, &m_vPos, &NextPos, t / 0.25f );
		a_pMonster->Set_Pos( Pos );
	}
}


VOID Chase::Exit( CMonster* a_pMonster )
{

}


VOID Chase::Initialize( TileMap::INFO* a_pMapInfo )
{
	m_pMapInfo = a_pMapInfo;
}


FLOAT Chase::GetDegree()
{
	static FLOAT fDegree[3][3] = {
		{ 225.0f, 270.0f, 315.0f },
		{ 180.0f, 0.0f,   0.0f },
		{ 135.0f, 90.0f,  45.0f }
	};

	INT iDirX = m_pCurrentPath->x - m_iCurrentX + 1;
	INT iDirZ = m_pCurrentPath->y - m_iCurrentZ + 1;

	return fDegree[iDirZ][iDirX];
}


BOOL Chase::isArrive( CMonster* a_pMonster )
{
	INT iX = static_cast<INT>( a_pMonster->Get_Pos().x );
	INT iZ = static_cast<INT>( a_pMonster->Get_Pos().z );

	return TRUE;
}