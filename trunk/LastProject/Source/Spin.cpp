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
	// Spin 공격 애니메이션으로 바꾼다.
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

	// 0.2초 마다 충돌 박스 생성
	if( a_pMonster->Get_AttackTime() >= 0.2f )
	{
		a_pMonster->Set_ClearAttackTime();

		// 공격 충돌 박스 생성
		a_pMonster->CreateAttackBoundBox();
	}

	// 공격이 끝났으면
	FLOAT t = a_pMonster->Get_Time();
	if( t >= 2.0f )
	{
		a_pMonster->Set_ClearTime();

		// 경직 상태로
		a_pMonster->GetFSM()->ChangeState( Groggy::GetInstance() );
	}
}


VOID Spin::Exit( CMonster* a_pMonster )
{

}