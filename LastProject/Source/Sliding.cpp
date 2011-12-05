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
	// Sliding �ִϸ��̼����� ����
	a_pMonster->ChangeAnimation( CMonster::ANIM_SLIDING );
	CDebugConsole::GetInstance()->Messagef( L"Sliding : ANIM_SLIDING \n" );
}


VOID Sliding::Execute( CMonster* a_pMonster )
{
	if( a_pMonster->Get_AnimationEndCheck() == FALSE )
	{
		// ���� �浹 �ڽ� ����
		a_pMonster->CreateAttackBoundBox();

		// ���� ���·�
		a_pMonster->GetFSM()->ChangeState( Stiffen::GetInstance() );
	}
}


VOID Sliding::Exit( CMonster* a_pMonster )
{

}