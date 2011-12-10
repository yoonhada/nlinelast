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
		// ��� �ִϸ��̼����� �ٲ۴�.
		a_pMonster->ChangeAnimation( CMonster::ANIM_STAND );

		// �������°� ������ �ִϸ��̼� ������ ���� ���� ��� ���·�
		a_pMonster->GetFSM()->ChangeState( WaitInterPolation::GetInstance() );
//	}
}


VOID Wait::Exit( CMonster* a_pMonster )
{

}