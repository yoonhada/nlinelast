#include "stdafx.h"

#include "Stand.h"
#include "Seek.h"
#include "Monster.h"


Stand* Stand::GetInstance()
{
	static Stand Instance;

	return &Instance;
}


VOID Stand::Enter( CMonster* a_pMonster )
{
	// Melee ���� �ִϸ��̼����� �ٲ۴�.
	a_pMonster->ChangeAnimation( CMonster::ANIM_STAND );
#ifdef _DEBUG
	CDebugConsole::GetInstance()->Messagef( L"Stand : ANIM_STAND \n" );
#endif
}


VOID Stand::Execute( CMonster* a_pMonster )
{
	a_pMonster->Set_UpdateTime();

	if( a_pMonster->Get_Time() >= 1.0f )
	{
		a_pMonster->Set_ClearTime();

		// Ž�� ���·�
		a_pMonster->GetFSM()->ChangeState( Seek::GetInstance() );
	}
}


VOID Stand::Exit( CMonster* a_pMonster )
{

}