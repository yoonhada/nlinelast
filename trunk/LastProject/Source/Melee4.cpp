#include "stdafx.h"

#include "Melee4.h"
#include "Stiffen.h"
#include "Monster.h"


Melee4* Melee4::GetInstance()
{
	static Melee4 Instance;

	return &Instance;
}


VOID Melee4::Enter( CMonster* a_pMonster )
{
	// Melee ���� �ִϸ��̼����� �ٲ۴�.
	a_pMonster->ChangeAnimation( CMonster::ANIM_MELEE_ATTACK4 );
#ifdef _DEBUG
	CDebugConsole::GetInstance()->Messagef( L"Melee2 : ANIM_MELEE_ATTACK4 \n" );
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


VOID Melee4::Execute( CMonster* a_pMonster )
{
	if( a_pMonster->Get_AnimationEndCheck() == FALSE )
	{
		// ���� �浹 �ڽ� ����
		a_pMonster->CreateAttackBoundBox();

		// ���� ���·�
		a_pMonster->GetFSM()->ChangeState( Stiffen::GetInstance() );
	}
}


VOID Melee4::Exit( CMonster* a_pMonster )
{

}