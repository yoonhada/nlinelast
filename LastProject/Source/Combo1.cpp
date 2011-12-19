#include "stdafx.h"

#include "Combo1.h"
#include "Combo1Stiffen.h"
#include "Monster.h"


Combo1* Combo1::GetInstance()
{
	static Combo1 Instance;

	return &Instance;
}


VOID Combo1::Enter( CMonster* a_pMonster )
{
	// Melee 공격 애니메이션으로 바꾼다.
	a_pMonster->ChangeAnimation( CMonster::ANIM_COMBO_ATTACK1 );
#ifdef _DEBUG
	CDebugConsole::GetInstance()->Messagef( L"Melee2 : ANIM_COMBO_ATTACK1 \n" );
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


VOID Combo1::Execute( CMonster* a_pMonster )
{
	if( a_pMonster->Get_AnimationEndCheck() == FALSE )
	{
		// 공격 충돌 박스 생성
		a_pMonster->CreateAttackBoundBox();

		// 경직 상태로
		a_pMonster->GetFSM()->ChangeState( Combo1Stiffen::GetInstance() );
	}
}


VOID Combo1::Exit( CMonster* a_pMonster )
{

}