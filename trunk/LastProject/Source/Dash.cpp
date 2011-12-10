#include "stdafx.h"

#include "Dash.h"
#include "Sliding.h"


Dash* Dash::GetInstance()
{
	static Dash Instance;

	return &Instance;
}


VOID Dash::Enter( CMonster* a_pMonster )
{
	a_pMonster->Set_DashData( a_pMonster->Get_Pos(), a_pMonster->Get_TargetPos() );
	a_pMonster->Set_InterpolationTime( a_pMonster->Get_TargetDistance() / 10.0f * 0.15f );

	if( CObjectManage::GetInstance()->IsHost() == TRUE )
	{
		CNetwork::GetInstance()->CS_Monster_Attack_Animation2( a_pMonster->Get_MonsterNumber(), CMonster::ANIM_DASH, a_pMonster->Get_Angle(), a_pMonster->Get_Pos(), a_pMonster->Get_TargetPos(), a_pMonster->Get_TargetDistance() );
	}
}


VOID Dash::Execute( CMonster* a_pMonster )
{
	a_pMonster->Set_UpdateTime();

	// 도착했으면 Stiffen 상태로
	FLOAT t = a_pMonster->Get_Time();
	if( t >= a_pMonster->Get_InterpolationTime() )
	{
		a_pMonster->Set_ClearTime();

		a_pMonster->GetFSM()->ChangeState( Sliding::GetInstance() );
	}
	else
	{
		D3DXVECTOR3 vPos( 0.0f, 0.0f, 0.0f );

		D3DXVec3Lerp( &vPos, &a_pMonster->Get_DashStartPos(), &a_pMonster->Get_DashEndPos(), t / a_pMonster->Get_InterpolationTime() );
		a_pMonster->Set_Pos( vPos );
	}
}


VOID Dash::Exit( CMonster* a_pMonster )
{

}