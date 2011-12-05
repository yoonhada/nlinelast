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
	m_vCurrentPos = a_pMonster->Get_Pos();
	m_vPreviousPos = m_vCurrentPos;
	m_iCurrentX = INT(  ( m_vCurrentPos.x + m_pMapInfo->vecEnd.x ) / m_pMapInfo->fTileSize );
	m_iCurrentZ = INT( -( m_vCurrentPos.z - m_pMapInfo->vecEnd.z ) / m_pMapInfo->fTileSize );
	m_pNextPath = a_pMonster->Get_Path();
	m_iTotalPathCnt = m_pNextPath->remainedNode;

	m_fCurrentAngle = D3DXToDegree( a_pMonster->Get_Angle() );
	m_fNextAngle = GetDegree();

	if( m_fCurrentAngle == 225.0f && m_fNextAngle == -90.0f )
	{
		m_fAngle0 = 225.0f;
		m_fAngle1 = 270.0f;
	}
	else if( m_fCurrentAngle == -90.0f && m_fNextAngle == 225.0f )
	{
		m_fAngle0 = 270.0f;
		m_fAngle1 = 225.0f;
	}
	else
	{
		m_fAngle0 = m_fCurrentAngle;
		m_fAngle1 = m_fNextAngle;
	}

	// 이동 애니메이션으로 바꾼다.
	a_pMonster->ChangeAnimation( CMonster::ANIM_MOVE );
}


VOID Chase::Execute( CMonster* a_pMonster )
{
	static FLOAT t = 0.0f;
	t += CFrequency::GetInstance()->getFrametime();

	if( t >= 0.25f )
	{
		t = 0.0f;

		// 현재 위치를 이전 위치에
		m_vPreviousPos = m_vCurrentPos;

		// 각도 오차값이 있으므로 최종 각도값을 넣어준다.
		a_pMonster->Set_Angle( D3DXToRadian( m_fNextAngle ) );

		m_iCurrentX = m_pNextPath->x;
		m_iCurrentZ = m_pNextPath->y;
 
		// 도착했으면 Seek 상태로
		m_pNextPath = m_pNextPath->next;
		if( m_pNextPath->remainedNode <= 0 )
		{
			if( CObjectManage::GetInstance()->IsHost() == TRUE )
			{
				a_pMonster->GetFSM()->ChangeState( Seek::GetInstance() );
			}
			else
			{
				// Path 제거
				Astar::GetInstance()->removePath( a_pMonster->Get_Path() );
				a_pMonster->Set_Path( NULL );
				a_pMonster->GetFSM()->ChangeState( NULL );
			}
		}
		else
		{
			m_vCurrentPos = a_pMonster->Get_Pos();

			m_fCurrentAngle = m_fNextAngle;
			m_fNextAngle = GetDegree();

			if( m_fCurrentAngle == 225.0f && m_fNextAngle == -90.0f )
			{
				m_fAngle0 = 225.0f;
				m_fAngle1 = 270.0f;
			}
			else if( m_fCurrentAngle == -90.0f && m_fNextAngle == 225.0f )
			{
				m_fAngle0 = 270.0f;
				m_fAngle1 = 225.0f;
			}
			else
			{
				m_fAngle0 = m_fCurrentAngle;
				m_fAngle1 = m_fNextAngle;
			}
		}
	}
	else
	{
		D3DXVECTOR3 pos( 0.0f, 0.0f, 0.0f );
		D3DXVECTOR2 vAngle( 0.0f, 0.0f );

		// 처음 이동할 Path인 경우
		if( m_iTotalPathCnt == m_pNextPath->remainedNode )
		{
			// 위치
			D3DXVECTOR3 p0 = m_vCurrentPos;
			D3DXVECTOR3 p1 = GetWorldPos( m_pNextPath->x, m_pNextPath->y );
			D3DXVECTOR3 p2 = GetWorldPos( m_pNextPath->next->x, m_pNextPath->next->y );

			D3DXVec3CatmullRom( &pos, &p0, &p0, &p1, &p2, t / 0.25f );

			// 각도
			D3DXVECTOR2 vCurrentAngle( 0.0f, m_fAngle0 );
			D3DXVECTOR2 vNextAngle( 0.0f, m_fAngle1 );

			D3DXVec2Lerp( &vAngle, &vCurrentAngle, &vNextAngle, t / 0.25f );
		}
		// 처음과 끝이 아닌 중간 Path
		else
		{
			// 위치
			D3DXVECTOR3 p0 = m_vPreviousPos;
			D3DXVECTOR3 p1 = m_vCurrentPos;
			D3DXVECTOR3 p2 = GetWorldPos( m_pNextPath->x, m_pNextPath->y );
			D3DXVECTOR3 p3 = GetWorldPos( m_pNextPath->next->x, m_pNextPath->next->y );

			D3DXVec3CatmullRom( &pos, &p0, &p1, &p2, &p3, t * 2.0f ); // / 0.5f );

			// 각도
			D3DXVECTOR2 vCurrentAngle( 0.0f, m_fAngle0 );
			D3DXVECTOR2 vNextAngle( 0.0f, m_fAngle1 );

			D3DXVec2Lerp( &vAngle, &vCurrentAngle, &vNextAngle, t * 2.0f ); // / 0.5f );
		}

		a_pMonster->Set_Pos( pos );
		a_pMonster->Set_Angle( D3DXToRadian( vAngle.y ) );
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
/*
	static FLOAT fDegree[3][3] = {
		{ 225.0f, 270.0f, 315.0f },
		{ 180.0f, 0.0f,   0.0f },
		{ 135.0f, 90.0f,  45.0f }
	};
*/
	static FLOAT fDegree[3][3] = {
		{ 135.0f, 180.0f, 225.0f },
		{  90.0f,   0.0f, -90.0f },
		{  45.0f,   0.0f, -45.0f }
	};

	INT iDirX = m_pNextPath->x - m_iCurrentX + 1;
	INT iDirZ = m_pNextPath->y - m_iCurrentZ + 1;

	return fDegree[iDirZ][iDirX];
}


D3DXVECTOR3 Chase::GetWorldPos( INT a_iX, INT a_iZ )
{
	D3DXVECTOR3 pos;

	pos.x = m_pMapInfo->vecStart.x + m_pMapInfo->fTileSize * a_iX + m_pMapInfo->fTileSize / 2;
	pos.y = 0.0f;
	pos.z = m_pMapInfo->vecEnd.z - m_pMapInfo->fTileSize * a_iZ - m_pMapInfo->fTileSize / 2;

	return pos;
}


