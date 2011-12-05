#include "stdafx.h"

#include "Spin.h"
#include "Stiffen.h"

#include "Monster.h"


Spin* Spin::GetInstance()
{
	static Spin Instance;

	return &Instance;
}


VOID Spin::Enter( CMonster* a_pMonster )
{
	// Spin 공격 애니메이션으로 바꾼다.
	a_pMonster->ChangeAnimation( CMonster::ANIM_SPIN_ATTACK );
	CDebugConsole::GetInstance()->Messagef( L"Spin : ANIM_SPIN_ATTACK \n" );
}


VOID Spin::Execute( CMonster* a_pMonster )
{
	// 경직 상태로
	a_pMonster->GetFSM()->ChangeState( Stiffen::GetInstance() );
}


VOID Spin::Exit( CMonster* a_pMonster )
{

}