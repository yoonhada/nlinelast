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


VOID Chase::Enter( CMonster* pMonster )
{
	m_pCurrentPath = pMonster->Get_Path();
	m_vPos = pMonster->Get_Pos();
	m_iCurrentX = INT( m_vPos.x + 510.0f ) / 10;
	m_iCurrentZ = INT( m_vPos.z + 510.0f ) / 10;
}


VOID Chase::Execute( CMonster* pMonster )
{
/*
	if( (INT)pMonster->Get_Pos().x != (INT)pMonster->Get_TargetPos().x && 
		(INT)pMonster->Get_Pos().z != (INT)pMonster->Get_TargetPos().z )
	{
		D3DXVECTOR3 pos( 0.0f, 0.0f, 0.0f );
		pos.x = pMonster->Get_Pos().x + pMonster->Get_TargetUnitVector().x * 20.0f * CFrequency::GetInstance()->getFrametime();
		pos.z = pMonster->Get_Pos().z + pMonster->Get_TargetUnitVector().z * 20.0f * CFrequency::GetInstance()->getFrametime();

		pMonster->Set_Pos( pos );
	}
	else
	{
		pMonster->GetFSM()->ChangeState( Seek::GetInstance() );
	}
*/
/*
	// 최종 위치에 도착했으면 탐색 상태로
	if( isArrive( pMonster ) )
	{
		pMonster->GetFSM()->ChangeState( Seek::GetInstance() );
	}
	else
	{
		
	}
*/

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
			pMonster->GetFSM()->ChangeState( Seek::GetInstance() );
		}
		else
		{
			// 다음 위치로 몬스터 각을 맞춘다.
			FLOAT fDegree = GetDegree();
			pMonster->Set_Angle( D3DXToRadian( fDegree - 90.0f ) );

			// 다음 Path 설정
			m_vPos = pMonster->Get_Pos();
		}
	}
	else
	{
		D3DXVECTOR3 Pos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		D3DXVECTOR3 NextPos = D3DXVECTOR3( -510.0f + 10.0f * m_pCurrentPath->x + 5.0f,
										   0.0f,
										   -510.0f + 10.0f * m_pCurrentPath->y + 5.0f );

		D3DXVec3Lerp( &Pos, &m_vPos, &NextPos, t / 0.25f );
		pMonster->Set_Pos( Pos );
	}
}


VOID Chase::Exit( CMonster* pMonster )
{

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


BOOL Chase::isArrive( CMonster* pMonster )
{
	INT iX = pMonster->Get_Pos().x;
	INT iZ = pMonster->Get_Pos().z;

	

	return TRUE;
}