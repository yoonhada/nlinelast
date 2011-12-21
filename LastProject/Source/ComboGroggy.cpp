#include "stdafx.h"

#include "ComboGroggy.h"
#include "Stand.h"

#include "Monster.h"


ComboGroggy* ComboGroggy::GetInstance()
{
	static ComboGroggy Instance;

	return &Instance;
}


VOID ComboGroggy::Enter( CMonster* a_pMonster )
{
	// Spin 공격 애니메이션으로 바꾼다.
	a_pMonster->ChangeAnimation( CMonster::ANIM_COMBO_GROGGY );
#ifdef _DEBUG
	CDebugConsole::GetInstance()->Messagef( L"ComboGroggy : ANIM_COMBO_GROGGY \n" );
#endif
}


VOID ComboGroggy::Execute( CMonster* a_pMonster )
{
	a_pMonster->Set_UpdateTime();

	// 그로기 상태가 끝났으면 다시 탐색 상태로
	FLOAT t = a_pMonster->Get_Time();
	if( t >= 8.0f )
	{
		a_pMonster->Set_ClearTime();

		// 호스트이면 탐색 상태로
		if( CObjectManage::GetInstance()->IsHost() == TRUE )
		{
			a_pMonster->GetFSM()->ChangeState( Stand::GetInstance() );
		}
		else
		{
			a_pMonster->GetFSM()->ChangeState( NULL );
		}
	}
}


VOID ComboGroggy::Exit( CMonster* a_pMonster )
{

}