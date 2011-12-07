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
/*
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

	D3DXVECTOR3 vNextPos = GetWorldPos( m_pNextPath->x, m_pNextPath->y );
	D3DXVECTOR3 vDistance = vNextPos - m_vCurrentPos;
	a_pMonster->Set_InterpolationTime( D3DXVec3Length( &vDistance ) / 10.0f * 0.25f );
*/
	// Chase 초기 정보 설정
	a_pMonster->Set_ChaseData();

	// 이동 애니메이션으로 바꾼다.
	a_pMonster->ChangeAnimation( CMonster::ANIM_MOVE );
	CDebugConsole::GetInstance()->Messagef( L"Chase : ANIM_MOVE \n" );
}


VOID Chase::Execute( CMonster* a_pMonster )
{
	a_pMonster->UpdateTime();

	FLOAT t = a_pMonster->Get_Time();

	if( t >= 0.25f )
	{
		a_pMonster->Set_ChaseNextData();
/*
		a_pMonster->ClearTime();

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

			D3DXVECTOR3 vNextPos = GetWorldPos( m_pNextPath->x, m_pNextPath->y );
			D3DXVECTOR3 vDistance = vNextPos - m_vCurrentPos;
			a_pMonster->Set_InterpolationTime( D3DXVec3Length( &vDistance ) / 10.0f * 0.25f );
		}
*/
	}
	else
	{
/*
		D3DXVECTOR3 vPos( 0.0f, 0.0f, 0.0f );
		D3DXVECTOR2 vAngle( 0.0f, 0.0f );

		// 처음 이동할 Path인 경우
		if( m_iTotalPathCnt == m_pNextPath->remainedNode )
		{
			// 위치
			D3DXVECTOR3 p0 = m_vCurrentPos;
			D3DXVECTOR3 p1 = GetWorldPos( m_pNextPath->x, m_pNextPath->y );
			D3DXVECTOR3 p2 = GetWorldPos( m_pNextPath->next->x, m_pNextPath->next->y );

			D3DXVec3CatmullRom( &vPos, &p0, &p0, &p1, &p2, t / a_pMonster->Get_InterpolationTime() );

			// 각도
			D3DXVECTOR2 vCurrentAngle( 0.0f, m_fAngle0 );
			D3DXVECTOR2 vNextAngle( 0.0f, m_fAngle1 );

			D3DXVec2Lerp( &vAngle, &vCurrentAngle, &vNextAngle, t / a_pMonster->Get_InterpolationTime() );
		}
		// 처음과 끝이 아닌 중간 Path
		else
		{
			// 위치
			D3DXVECTOR3 p0 = m_vPreviousPos;
			D3DXVECTOR3 p1 = m_vCurrentPos;
			D3DXVECTOR3 p2 = GetWorldPos( m_pNextPath->x, m_pNextPath->y );
			D3DXVECTOR3 p3 = GetWorldPos( m_pNextPath->next->x, m_pNextPath->next->y );

			D3DXVec3CatmullRom( &vPos, &p0, &p1, &p2, &p3, t / a_pMonster->Get_InterpolationTime() );

			// 각도
			D3DXVECTOR2 vCurrentAngle( 0.0f, m_fAngle0 );
			D3DXVECTOR2 vNextAngle( 0.0f, m_fAngle1 );

			D3DXVec2Lerp( &vAngle, &vCurrentAngle, &vNextAngle, t / a_pMonster->Get_InterpolationTime() );
		}

		a_pMonster->Set_Pos( vPos );
		a_pMonster->Set_Angle( D3DXToRadian( vAngle.y ) );
	}
*/
		D3DXVECTOR3 vPos( 0.0f, 0.0f, 0.0f );
		D3DXVECTOR2 vAngle( 0.0f, 0.0f );

		// 처음 이동할 Path인 경우
		if( a_pMonster->Get_TotalPathCnt() == a_pMonster->Get_NextPath()->remainedNode )
		{
			// 위치
			D3DXVECTOR3 p0 = a_pMonster->Get_CurrentPos();
			D3DXVECTOR3 p1 = a_pMonster->GetWorldPos( a_pMonster->Get_NextPath()->x, a_pMonster->Get_NextPath()->y );
			D3DXVECTOR3 p2 = a_pMonster->GetWorldPos( a_pMonster->Get_NextPath()->next->x, a_pMonster->Get_NextPath()->next->y );

			D3DXVec3CatmullRom( &vPos, &p0, &p0, &p1, &p2, t / a_pMonster->Get_InterpolationTime() );

			// 각도
			D3DXVECTOR2 vCurrentAngle( 0.0f, a_pMonster->Get_Angle0() );
			D3DXVECTOR2 vNextAngle( 0.0f, a_pMonster->Get_Angle1() );

			D3DXVec2Lerp( &vAngle, &vCurrentAngle, &vNextAngle, t / a_pMonster->Get_InterpolationTime() );
		}
		// 처음과 끝이 아닌 중간 Path
		else
		{
			// 위치
			D3DXVECTOR3 p0 = a_pMonster->Get_PreviousPos();
			D3DXVECTOR3 p1 = a_pMonster->Get_CurrentPos();
			D3DXVECTOR3 p2 = a_pMonster->GetWorldPos( a_pMonster->Get_NextPath()->x, a_pMonster->Get_NextPath()->y );
			D3DXVECTOR3 p3 = a_pMonster->GetWorldPos( a_pMonster->Get_NextPath()->next->x, a_pMonster->Get_NextPath()->next->y );

			D3DXVec3CatmullRom( &vPos, &p0, &p1, &p2, &p3, t / a_pMonster->Get_InterpolationTime() );

			// 각도
			D3DXVECTOR2 vCurrentAngle( 0.0f, a_pMonster->Get_Angle0() );
			D3DXVECTOR2 vNextAngle( 0.0f, a_pMonster->Get_Angle1() );

			D3DXVec2Lerp( &vAngle, &vCurrentAngle, &vNextAngle, t / a_pMonster->Get_InterpolationTime() );
		}

		a_pMonster->Set_Pos( vPos );
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

/*
FLOAT Chase::GetDegree()
{
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
*/

