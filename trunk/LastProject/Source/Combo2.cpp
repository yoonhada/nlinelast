#include "stdafx.h"

#include "Combo2.h"
#include "Combo2Stiffen.h"
#include "Monster.h"


Combo2* Combo2::GetInstance()
{
	static Combo2 Instance;

	return &Instance;
}


VOID Combo2::Enter( CMonster* a_pMonster )
{
	// Melee ���� �ִϸ��̼����� �ٲ۴�.
	a_pMonster->ChangeAnimation( CMonster::ANIM_COMBO_ATTACK2 );
#ifdef _DEBUG
	CDebugConsole::GetInstance()->Messagef( L"Melee2 : ANIM_COMBO_ATTACK2 \n" );
#endif

	CSound::GetInstance()->PlayEffect( CSound::EFFECT_CLOWN_ATTACK1 + rand() % 3 );	
}


VOID Combo2::Execute( CMonster* a_pMonster )
{
	if( a_pMonster->Get_AnimationEndCheck() == FALSE )
	{
		// ���� �浹 �ڽ� ����
		a_pMonster->CreateAttackBoundBox();

		// ���� ���·�
		a_pMonster->GetFSM()->ChangeState( Combo2Stiffen::GetInstance() );
	}
}


VOID Combo2::Exit( CMonster* a_pMonster )
{

}