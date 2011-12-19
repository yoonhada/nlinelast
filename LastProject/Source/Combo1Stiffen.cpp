#include "stdafx.h"

#include "Combo1Stiffen.h"
#include "Combo2.h"

#include "Monster.h"


Combo1Stiffen* Combo1Stiffen::GetInstance()
{
	static Combo1Stiffen Instance;

	return &Instance;
}


VOID Combo1Stiffen::Enter( CMonster* a_pMonster )
{

}


VOID Combo1Stiffen::Execute( CMonster* a_pMonster )
{
	a_pMonster->Set_UpdateTime();

	if( a_pMonster->Get_Time() >= 2.0f )
	{
		a_pMonster->Set_ClearTime();

		// ��� �ִϸ��̼����� �ٲ۴�.
//		a_pMonster->ChangeAnimation( CMonster::ANIM_STAND );

		// �޺�2
		a_pMonster->GetFSM()->ChangeState( Combo2::GetInstance() );
	}
}


VOID Combo1Stiffen::Exit( CMonster* a_pMonster )
{

}