#include "stdafx.h"

#include "Wait.h"
#include "WaitInterPolation.h"

#include "Monster.h"


Wait* Wait::GetInstance()
{
	static Wait Instance;

	return &Instance;
}


VOID Wait::Enter( CMonster* a_pMonster )
{

}


VOID Wait::Execute( CMonster* a_pMonster )
{
/*
	a_pMonster->Set_UpdateTime();

	if( a_pMonster->Get_Time() >= 0.001f )
	{
		a_pMonster->Set_ClearTime();
*/
		// 대기 애니메이션으로 바꾼다.
		a_pMonster->ChangeAnimation( CMonster::ANIM_STAND );

		// 경직상태가 끝나면 애니메이션 보간을 위해 보간 대기 상태로
		a_pMonster->GetFSM()->ChangeState( WaitInterPolation::GetInstance() );
//	}
}


VOID Wait::Exit( CMonster* a_pMonster )
{

}