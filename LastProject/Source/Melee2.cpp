#include "stdafx.h"

#include "Melee2.h"
#include "Stiffen.h"

#include "Monster.h"


Melee2* Melee2::GetInstance()
{
	static Melee2 Instance;

	return &Instance;
}


VOID Melee2::Enter( CMonster* a_pMonster )
{
	// Melee ���� �ִϸ��̼����� �ٲ۴�.
	a_pMonster->ChangeAnimation( CMonster::ANIM_MELEE_ATTACK2 );
	CDebugConsole::GetInstance()->Messagef( L"Melee2 : ANIM_MELEE_ATTACK2 \n" );
}


VOID Melee2::Execute( CMonster* a_pMonster )
{
	if( a_pMonster->Get_AnimationEndCheck() == FALSE )
	{
		// ���� �浹 �ڽ� ����
		a_pMonster->CreateAttackBoundBox();

		// ���� ���·�
		a_pMonster->GetFSM()->ChangeState( Stiffen::GetInstance() );
	}
}


VOID Melee2::Exit( CMonster* a_pMonster )
{

}