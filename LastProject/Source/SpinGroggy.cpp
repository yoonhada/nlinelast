#include "stdafx.h"

#include "SpinGroggy.h"
#include "Seek.h"

#include "Monster.h"


SpinGroggy* SpinGroggy::GetInstance()
{
	static SpinGroggy Instance;

	return &Instance;
}


VOID SpinGroggy::Enter( CMonster* a_pMonster )
{
	// Spin 공격 애니메이션으로 바꾼다.
	a_pMonster->ChangeAnimation( CMonster::ANIM_SPIN_GROGGY );
#ifdef _DEBUG
	CDebugConsole::GetInstance()->Messagef( L"Groggy : ANIM_SPIN_GROGGY \n" );
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


VOID SpinGroggy::Execute( CMonster* a_pMonster )
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


VOID SpinGroggy::Exit( CMonster* a_pMonster )
{

}