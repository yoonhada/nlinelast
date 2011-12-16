#include "stdafx.h"

#include "Wait.h"
#include "Seek.h"

#include "Monster.h"


Wait* Wait::GetInstance()
{
	static Wait Instance;

	return &Instance;
}


VOID Wait::Enter( CMonster* a_pMonster )
{
	a_pMonster->ChangeAnimation( CMonster::ANIM_STAND );
}


VOID Wait::Execute( CMonster* a_pMonster )
{
	a_pMonster->Set_UpdateTime();

	if( a_pMonster->Get_Time() >= 1.0f )
	{
		a_pMonster->Set_ClearTime();

		a_pMonster->GetFSM()->ChangeState( Seek::GetInstance() );
	}
}


VOID Wait::Exit( CMonster* a_pMonster )
{

}