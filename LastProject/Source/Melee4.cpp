#include "stdafx.h"

#include "Melee4.h"
#include "Stiffen.h"
#include "Monster.h"


Melee4* Melee4::GetInstance()
{
	static Melee4 Instance;

	return &Instance;
}


VOID Melee4::Enter( CMonster* a_pMonster )
{
	// Melee 공격 애니메이션으로 바꾼다.
	a_pMonster->ChangeAnimation( CMonster::ANIM_MELEE_ATTACK4 );
#ifdef _DEBUG
	CDebugConsole::GetInstance()->Messagef( L"Melee2 : ANIM_MELEE_ATTACK4 \n" );
#endif

	if( a_pMonster->Get_MonsterNumber() == 2 )
	{
		CSound::GetInstance()->PlayEffect( CSound::EFFECT_CLOWN_ATTACK1 + rand() % 3 );
	}
	else
	{
		CSound::GetInstance()->PlayEffect( CSound::EFFECT_PANDA_ATTACK1 );
	}
}


VOID Melee4::Execute( CMonster* a_pMonster )
{
	if( a_pMonster->Get_AnimationEndCheck() == FALSE )
	{
		// 공격 충돌 박스 생성
		a_pMonster->CreateAttackBoundBox();

		// 경직 상태로
		a_pMonster->GetFSM()->ChangeState( Stiffen::GetInstance() );
	}
}


VOID Melee4::Exit( CMonster* a_pMonster )
{

}