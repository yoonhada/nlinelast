#include "stdafx.h"

#include "Spin.h"
#include "Groggy.h"

#include "Monster.h"


Spin* Spin::GetInstance()
{
	static Spin Instance;

	return &Instance;
}


VOID Spin::Enter( CMonster* a_pMonster )
{
	// Spin ���� �ִϸ��̼����� �ٲ۴�.
	a_pMonster->ChangeAnimation( CMonster::ANIM_SPIN_ATTACK );
#ifdef _DEBUG
	CDebugConsole::GetInstance()->Messagef( L"Spin : ANIM_SPIN_ATTACK \n" );
#endif

	CSound::GetInstance()->PlayEffect( 0 );
}


VOID Spin::Execute( CMonster* a_pMonster )
{
	a_pMonster->Set_UpdateTime();
	a_pMonster->Set_UpdateAttackTime();

	// 0.2�� ���� �浹 �ڽ� ����
	if( a_pMonster->Get_AttackTime() >= 0.2f )
	{
		a_pMonster->Set_ClearAttackTime();

		// ���� �浹 �ڽ� ����
		a_pMonster->CreateAttackBoundBox();
	}

	// ������ ��������
	FLOAT t = a_pMonster->Get_Time();
	if( t >= 2.0f )
	{
		a_pMonster->Set_ClearTime();

		// ���� ���·�
		a_pMonster->GetFSM()->ChangeState( Groggy::GetInstance() );
	}
}


VOID Spin::Exit( CMonster* a_pMonster )
{

}