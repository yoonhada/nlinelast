#include "stdafx.h"

#include "Senpukyaku.h"
#include "SpinGroggy.h"

#include "Monster.h"


Senpukyaku* Senpukyaku::GetInstance()
{
	static Senpukyaku Instance;

	return &Instance;
}


VOID Senpukyaku::Enter( CMonster* a_pMonster )
{
	// Spin 공격 애니메이션으로 바꾼다.
	a_pMonster->ChangeAnimation( CMonster::ANIM_SENPUKYAKU );
#ifdef _DEBUG
	//CDebugConsole::GetInstance()->Messagef( L"Spin : ANIM_SENPUKYAKU \n" );
#endif
}


VOID Senpukyaku::Execute( CMonster* a_pMonster )
{
	a_pMonster->Set_UpdateTime();
	a_pMonster->Set_UpdateAttackTime();

	// 0.2초 마다 충돌 박스 생성
	if( a_pMonster->Get_AttackTime() >= 0.2f )
	{
		a_pMonster->Set_ClearAttackTime();

		// 공격 충돌 박스 생성
		a_pMonster->CreateAttackBoundBox();

		CSound::GetInstance()->PlayEffect( CSound::EFFECT_SWING_BIG );
	}

	// 공격이 끝났으면
	FLOAT t = a_pMonster->Get_Time();
	if( t >= 4.0f )
	{
		a_pMonster->Set_ClearTime();

		// 경직 상태로
		a_pMonster->GetFSM()->ChangeState( SpinGroggy::GetInstance() );
	}
}


VOID Senpukyaku::Exit( CMonster* a_pMonster )
{

}