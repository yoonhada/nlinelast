#include "stdafx.h"

#include "Senpukyaku.h"
#include "SpinGroggy.h"

#include "Monster.h"


Senpukyaku* Senpukyaku::GetInstance()
{
	static Senpukyaku Instance;

	return &Instance;
}


VOID Senpukyaku::Enter( CMonster* a_pMonster )
{
	// Spin ���� �ִϸ��̼����� �ٲ۴�.
	a_pMonster->ChangeAnimation( CMonster::ANIM_SENPUKYAKU );
#ifdef _DEBUG
	//CDebugConsole::GetInstance()->Messagef( L"Spin : ANIM_SENPUKYAKU \n" );
#endif
}


VOID Senpukyaku::Execute( CMonster* a_pMonster )
{
	a_pMonster->Set_UpdateTime();
	a_pMonster->Set_UpdateAttackTime();

	// 0.2�� ���� �浹 �ڽ� ����
	if( a_pMonster->Get_AttackTime() >= 0.2f )
	{
		a_pMonster->Set_ClearAttackTime();

		// ���� �浹 �ڽ� ����
		a_pMonster->CreateAttackBoundBox();

		CSound::GetInstance()->PlayEffect( CSound::EFFECT_SWING_BIG );
	}

	// ������ ��������
	FLOAT t = a_pMonster->Get_Time();
	if( t >= 4.0f )
	{
		a_pMonster->Set_ClearTime();

		// ���� ���·�
		a_pMonster->GetFSM()->ChangeState( SpinGroggy::GetInstance() );
	}
}


VOID Senpukyaku::Exit( CMonster* a_pMonster )
{

}