#include "stdafx.h"

#include "Sliding.h"
#include "Monster.h"
#include "Stiffen.h"


Sliding* Sliding::GetInstance()
{
	static Sliding Instance;

	return &Instance;
}


VOID Sliding::Enter( CMonster* a_pMonster )
{
	// Sliding 애니메이션으로 변경
	a_pMonster->ChangeAnimation( CMonster::ANIM_SLIDING );
	CDebugConsole::GetInstance()->Messagef( L"Sliding : ANIM_SLIDING \n" );
}


VOID Sliding::Execute( CMonster* a_pMonster )
{
	if( a_pMonster->Get_AnimationEndCheck() == FALSE )
	{
		// 공격 충돌 박스 생성
		a_pMonster->CreateAttackBoundBox();

		// 경직 상태로
		a_pMonster->GetFSM()->ChangeState( Stiffen::GetInstance() );
	}
}


VOID Sliding::Exit( CMonster* a_pMonster )
{

}