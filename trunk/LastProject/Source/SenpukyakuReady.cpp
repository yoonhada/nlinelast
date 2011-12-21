#include "stdafx.h"

#include "SenpukyakuReady.h"
#include "Senpukyaku.h"

#include "Monster.h"


SenpukyakuReady* SenpukyakuReady::GetInstance()
{
	static SenpukyakuReady Instance;

	return &Instance;
}


VOID SenpukyakuReady::Enter( CMonster* a_pMonster )
{
	// Spin 공격 애니메이션으로 바꾼다.
	a_pMonster->ChangeAnimation( CMonster::ANIM_SENPUKYAKU_READY );
#ifdef _DEBUG
	//CDebugConsole::GetInstance()->Messagef( L"Spin : ANIM_SENPUKYAKU_READY \n" );
#endif
}


VOID SenpukyakuReady::Execute( CMonster* a_pMonster )
{
	// 애니메이션 보간이 끝나면 회전 공격
	if( a_pMonster->Get_ChangingAnimation() == FALSE )
	{
		a_pMonster->GetFSM()->ChangeState( Senpukyaku::GetInstance() );
	}
}


VOID SenpukyakuReady::Exit( CMonster* a_pMonster )
{

}