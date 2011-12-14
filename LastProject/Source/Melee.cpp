#include "stdafx.h"

#include "Melee.h"
#include "Stiffen.h"
#include "Monster.h"


Melee* Melee::GetInstance()
{
	static Melee Instance;

	return &Instance;
}


VOID Melee::Enter( CMonster* a_pMonster )
{
	// Melee ���� �ִϸ��̼����� �ٲ۴�.
	a_pMonster->ChangeAnimation( /*CMonster::ANIM_MELEE_ATTACK*/10 );
#ifdef _DEBUG
	CDebugConsole::GetInstance()->Messagef( L"Melee : ANIM_MELEE_ATTACK \n" );
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


VOID Melee::Execute( CMonster* a_pMonster )
{
	if( a_pMonster->Get_AnimationEndCheck() == FALSE )
	{
		// ���� �浹 �ڽ� ����
		a_pMonster->CreateAttackBoundBox();
		
		// ���� ���·�
		a_pMonster->GetFSM()->ChangeState( Stiffen::GetInstance() );
	}
}


VOID Melee::Exit( CMonster* a_pMonster )
{

}