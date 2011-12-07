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
	}
	else
	{
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
