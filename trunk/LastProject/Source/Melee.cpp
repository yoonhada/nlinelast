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
	a_pMonster->ChangeAnimation( 2 );
}


VOID Melee::Execute( CMonster* a_pMonster )
{
	if( a_pMonster->Get_AnimationEndCheck() == FALSE )
	{
		// 공격 충돌 박스 생성
		a_pMonster->CreateAttackBoundBox();

		// 경직 상태로
		a_pMonster->GetFSM()->ChangeState( Stiffen::GetInstance() );
	}
}


VOID Melee::Exit( CMonster* a_pMonster )
{

}