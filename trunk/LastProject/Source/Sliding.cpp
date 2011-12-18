#include "stdafx.h"

#include "Sliding.h"
#include "Stiffen.h"
#include "Monster.h"


Sliding* Sliding::GetInstance()
{
	static Sliding Instance;

	return &Instance;
}


VOID Sliding::Enter( CMonster* a_pMonster )
{
	// Sliding �ִϸ��̼����� ����
	a_pMonster->ChangeAnimation( CMonster::ANIM_SLIDING );

	if( a_pMonster->Get_MonsterNumber() == 2 )
	{
		CSound::GetInstance()->PlayEffect( 0 );
	}
	else
	{
		CSound::GetInstance()->PlayEffect( 0 );
	}
#ifdef _DEBUG
	CDebugConsole::GetInstance()->Messagef( L"Sliding : ANIM_SLIDING \n" );
#endif
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