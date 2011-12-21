#include "stdafx.h"

#include "Melee3.h"
#include "Stiffen.h"
#include "Monster.h"


Melee3* Melee3::GetInstance()
{
	static Melee3 Instance;

	return &Instance;
}


VOID Melee3::Enter( CMonster* a_pMonster )
{
	// Melee ���� �ִϸ��̼����� �ٲ۴�.
	a_pMonster->ChangeAnimation( CMonster::ANIM_MELEE_ATTACK3 );
#ifdef _DEBUG
	CDebugConsole::GetInstance()->Messagef( L"Melee2 : ANIM_MELEE_ATTACK3 \n" );
#endif

	if( a_pMonster->Get_MonsterNumber() == 2 )
	{
		CSound::GetInstance()->PlayEffect( CSound::EFFECT_CLOWN_ATTACK1 + rand() % 3 );
	}
	else
	{
		CSound::GetInstance()->PlayEffect( CSound::EFFECT_PANDA_ATTACK1 );
	}
}


VOID Melee3::Execute( CMonster* a_pMonster )
{
	if( a_pMonster->Get_AnimationEndCheck() == FALSE )
	{
		// ���� �浹 �ڽ� ����
		a_pMonster->CreateAttackBoundBox();

		// ���� ���·�
		a_pMonster->GetFSM()->ChangeState( Stiffen::GetInstance() );
	}
}


VOID Melee3::Exit( CMonster* a_pMonster )
{

}