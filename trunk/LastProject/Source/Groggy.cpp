#include "stdafx.h"

#include "Groggy.h"
#include "Seek.h"

#include "Monster.h"


Groggy* Groggy::GetInstance()
{
	static Groggy Instance;

	return &Instance;
}


VOID Groggy::Enter( CMonster* a_pMonster )
{
	// Spin 공격 애니메이션으로 바꾼다.
	a_pMonster->ChangeAnimation( CMonster::ANIM_GROGGY );
#ifdef _DEBUG
	CDebugConsole::GetInstance()->Messagef( L"Groggy : ANIM_GROGGY \n" );
#endif

	if( a_pMonster->Get_MonsterNumber() == 2 )
	{
		CSound::GetInstance()->PlayEffect( 0 );
	}
	else
	{
		CSound::GetInstance()->PlayEffect( 0 );
	}
}


VOID Groggy::Execute( CMonster* a_pMonster )
{
	a_pMonster->Set_UpdateTime();

	// 그로기 상태가 끝났으면 다시 탐색 상태로
	FLOAT t = a_pMonster->Get_Time();
	if( t >= 4.0f )
	{
		a_pMonster->Set_ClearTime();

		// 호스트이면 탐색 상태로
		if( CObjectManage::GetInstance()->IsHost() == TRUE )
		{
			a_pMonster->GetFSM()->ChangeState( Seek::GetInstance() );
		}
		else
		{
			a_pMonster->GetFSM()->ChangeState( NULL );
		}
	}
}


VOID Groggy::Exit( CMonster* a_pMonster )
{

}