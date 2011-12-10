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
	// Chase �ʱ� ���� ����
	a_pMonster->Set_ChaseData();

	// �̵� �ִϸ��̼����� �ٲ۴�.
	a_pMonster->ChangeAnimation( CMonster::ANIM_MOVE );
#ifdef _DEBUG
	CDebugConsole::GetInstance()->Messagef( L"Chase : ANIM_MOVE \n" );
#endif
}


VOID Chase::Execute( CMonster* a_pMonster )
{
	a_pMonster->Set_UpdateTime();

	FLOAT t = a_pMonster->Get_Time();

	if( t >= 0.25f )
	{
		a_pMonster->Set_ClearTime();

		a_pMonster->Set_ChaseNextData();
	}
	else
	{
		D3DXVECTOR3 vPos( 0.0f, 0.0f, 0.0f );
		D3DXVECTOR2 vAngle( 0.0f, 0.0f );

		// ó�� �̵��� Path�� ���
		if( a_pMonster->Get_ChaseTotalPathCnt() == a_pMonster->Get_ChaseNextPath()->remainedNode )
		{
			// ��ġ
			D3DXVECTOR3 p0 = a_pMonster->Get_ChaseCurrentPos();
			D3DXVECTOR3 p1 = a_pMonster->GetWorldPos( a_pMonster->Get_ChaseNextPath()->x, a_pMonster->Get_ChaseNextPath()->y );
			D3DXVECTOR3 p2 = a_pMonster->GetWorldPos( a_pMonster->Get_ChaseNextPath()->next->x, a_pMonster->Get_ChaseNextPath()->next->y );

			D3DXVec3CatmullRom( &vPos, &p0, &p0, &p1, &p2, t / 0.25f );

			// ����
			D3DXVECTOR2 vCurrentAngle( 0.0f, a_pMonster->Get_ChaseAngle0() );
			D3DXVECTOR2 vNextAngle( 0.0f, a_pMonster->Get_ChaseAngle1() );

			D3DXVec2Lerp( &vAngle, &vCurrentAngle, &vNextAngle, t / 0.25f );
		}
		// ó���� ���� �ƴ� �߰� Path
		else
		{
			// ��ġ
			D3DXVECTOR3 p0 = a_pMonster->Get_ChasePreviousPos();
			D3DXVECTOR3 p1 = a_pMonster->Get_ChaseCurrentPos();
			D3DXVECTOR3 p2 = a_pMonster->GetWorldPos( a_pMonster->Get_ChaseNextPath()->x, a_pMonster->Get_ChaseNextPath()->y );
			D3DXVECTOR3 p3 = a_pMonster->GetWorldPos( a_pMonster->Get_ChaseNextPath()->next->x, a_pMonster->Get_ChaseNextPath()->next->y );

			D3DXVec3CatmullRom( &vPos, &p0, &p1, &p2, &p3, t / 0.25f );

			// ����
			D3DXVECTOR2 vCurrentAngle( 0.0f, a_pMonster->Get_ChaseAngle0() );
			D3DXVECTOR2 vNextAngle( 0.0f, a_pMonster->Get_ChaseAngle1() );

			D3DXVec2Lerp( &vAngle, &vCurrentAngle, &vNextAngle, t / 0.25f );
		}

		a_pMonster->Set_Pos( vPos );
		a_pMonster->Set_Angle( D3DXToRadian( vAngle.y ) );
	}
}


VOID Chase::Exit( CMonster* a_pMonster )
{

}