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

#ifdef _DEBUG
	CDebugConsole::GetInstance()->Messagef( L"Sliding : ANIM_SLIDING \n" );
#endif
}


VOID Sliding::Execute( CMonster* a_pMonster )
{
	// ����
	if( a_pMonster->Get_MonsterNumber() == 2 )
	{
		a_pMonster->Set_UpdateSoundTime();
		FLOAT fSoundTime = a_pMonster->Get_SoundTime();
		if( fSoundTime >= 1.0f )
		{
			a_pMonster->Set_ClearSoundTime();
			CSound::GetInstance()->PlayEffect( CSound::EFFECT_CLOWN_DOWN );

			// ���� �浹 �ڽ� ����
			a_pMonster->CreateAttackBoundBox();
		}
	}
	

	if( a_pMonster->Get_AnimationEndCheck() == FALSE )
	{
		a_pMonster->Set_ClearSoundTime();

		// ���� ���·�
		a_pMonster->GetFSM()->ChangeState( Stiffen::GetInstance() );
/*
		if( a_pMonster->Get_MonsterNumber() == 2 )
		{
			CSound::GetInstance()->PlayEffect( CSound::EFFECT_CLOWN_DOWN );
		}*/
	}
}


VOID Sliding::Exit( CMonster* a_pMonster )
{

}