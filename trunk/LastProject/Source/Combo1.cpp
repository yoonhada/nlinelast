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
	// Melee ���� �ִϸ��̼����� �ٲ۴�.
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
		// ���� �浹 �ڽ� ����
		a_pMonster->CreateAttackBoundBox();

		// ���� ���·�
		a_pMonster->GetFSM()->ChangeState( Combo1Stiffen::GetInstance() );
	}
}


VOID Combo1::Exit( CMonster* a_pMonster )
{

}