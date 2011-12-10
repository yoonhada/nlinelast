#include "stdafx.h"

#include "Battle.h"
#include "Seek.h"
#include "LockOn.h"
#include "Spin.h"
#include "DashReady.h"

#include "Charactor.h"
#include "Monster.h"


Battle* Battle::GetInstance()
{
	static Battle Instance;

	return &Instance;
}


VOID Battle::Enter( CMonster* a_pMonster )
{

}


VOID Battle::Execute( CMonster* a_pMonster )
{
	INT Target = -1;
	D3DXVECTOR3 pos;
	FLOAT length = 0.0f;
	static FLOAT min = 0.0f;

	FLOAT fDistance = a_pMonster->Get_TargetDistance();

#ifdef _DEBUG
	// 타겟 번호
	static FLOAT target = 0;
	target = static_cast<FLOAT>( a_pMonster->Get_Target() );
	CDebugInterface::GetInstance()->AddMessageFloat( "target", target );
#endif

	// 근접 공격범위 안에 들어왔으면 밀리나 회전 공격중 랜덤 선택
	if( fDistance < 25.0f )
	{
		// 근접 공격
		if( FastRand2() < 0.5f )
		{
			a_pMonster->GetFSM()->ChangeState( LockOn::GetInstance() );
		}
		// 회전 공격
		else
		{
			a_pMonster->GetFSM()->ChangeState( Spin::GetInstance() );
		}
	}
	// 대시 공격 범위면
	else if( fDistance > 80.0f && fDistance < 120.0f )
	{
		CNetwork::GetInstance()->CS_Monster_Attack_Animation( a_pMonster->Get_MonsterNumber(), CMonster::ANIM_DASH );
		a_pMonster->GetFSM()->ChangeState( DashReady::GetInstance() );
	}

	// 공격범위가 아니면 다시 탐색 상태로
	else
	{
		a_pMonster->GetFSM()->ChangeState( Seek::GetInstance() );
	}
}


VOID Battle::Exit( CMonster* a_pMonster )
{

}