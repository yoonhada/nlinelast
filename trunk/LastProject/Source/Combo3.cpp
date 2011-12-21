#include "stdafx.h"

#include "Combo3.h"
#include "Stiffen.h"
#include "Monster.h"


Combo3* Combo3::GetInstance()
{
	static Combo3 Instance;

	return &Instance;
}


VOID Combo3::Enter( CMonster* a_pMonster )
{
	// Melee ���� �ִϸ��̼����� �ٲ۴�.
	a_pMonster->ChangeAnimation( CMonster::ANIM_COMBO_ATTACK3 );
#ifdef _DEBUG
	CDebugConsole::GetInstance()->Messagef( L"Melee2 : ANIM_COMBO_ATTACK3 \n" );
#endif

	CSound::GetInstance()->PlayEffect( CSound::EFFECT_CLOWN_ATTACK1 + rand() % 3 );	
}


VOID Combo3::Execute( CMonster* a_pMonster )
{
	if( a_pMonster->Get_AnimationEndCheck() == FALSE )
	{
		// ���� �浹 �ڽ� ����
		a_pMonster->CreateAttackBoundBox();

		// ���� ���·�
		a_pMonster->GetFSM()->ChangeState( Stiffen::GetInstance() );
	}
}


VOID Combo3::Exit( CMonster* a_pMonster )
{

}