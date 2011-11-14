#include "stdafx.h"

#include "Battle.h"
#include "Seek.h"
#include "LockOn.h"
#include "Melee.h"
#include "Spin.h"
#include "Dash.h"

#include "Charactor.h"
#include "Monster.h"


Battle* Battle::GetInstance()
{
	static Battle Instance;

	return &Instance;
}


VOID Battle::Enter( CMonster* pMonster )
{

}


VOID Battle::Execute( CMonster* pMonster )
{
	INT Target = -1;
	D3DXVECTOR3 pos;
	FLOAT length = 0.0f;
	static FLOAT min = 0.0f;

	FLOAT fDistance = pMonster->Get_TargetDistance();

	// 타겟 번호
	static FLOAT target;
	target = pMonster->Get_Target();
	CDebugInterface::GetInstance()->AddMessageFloat( "target", target );

	// 근접 공격범위 안에 들어왔으면 밀리나 회전 공격중 랜덤 선택
	if( fDistance <= 50.0f )
	{
		// 근접 공격
		//if( FastRand2() < 0.5f )
		//{
			pMonster->GetFSM()->ChangeState( LockOn::GetInstance() );
		//}
		// 회전 공격
		/*else
		{
			pMonster->GetFSM()->ChangeState( Spin::GetInstance() );
		}	*/
	}
/*
	// 대시 공격 범위면
	else if( fDistance > 100.0f && fDistance < 300.0f )
	{
		pMonster->GetFSM()->ChangeState( Dash::GetInstance() );
	}
*/
	// 공격범위가 아니면 다시 탐색 상태로
	else
	{
		pMonster->GetFSM()->ChangeState( Seek::GetInstance() );
	}
}


VOID Battle::Exit( CMonster* pMonster )
{

}