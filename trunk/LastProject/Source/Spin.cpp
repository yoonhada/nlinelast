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
	// Spin ���� �ִϸ��̼����� �ٲ۴�.
	a_pMonster->ChangeAnimation( CMonster::ANIM_SPIN_ATTACK );
	CDebugConsole::GetInstance()->Messagef( L"Spin : ANIM_SPIN_ATTACK \n" );
}


VOID Spin::Execute( CMonster* a_pMonster )
{
	// ���� ���·�
	a_pMonster->GetFSM()->ChangeState( Stiffen::GetInstance() );
}


VOID Spin::Exit( CMonster* a_pMonster )
{

}