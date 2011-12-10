#include "stdafx.h"

#include "Stiffen.h"
#include "WaitInterPolation.h"

#include "Monster.h"


Stiffen* Stiffen::GetInstance()
{
	static Stiffen Instance;

	return &Instance;
}


VOID Stiffen::Enter( CMonster* a_pMonster )
{

}


VOID Stiffen::Execute( CMonster* a_pMonster )
{
	a_pMonster->Set_UpdateTime();

	if( a_pMonster->Get_Time() >= 2.0f )
	{
		a_pMonster->Set_ClearTime();

		// ��� �ִϸ��̼����� �ٲ۴�.
		a_pMonster->ChangeAnimation( CMonster::ANIM_STAND );

		// �������°� ������ �ִϸ��̼� ������ ���� ���� ��� ���·�
		a_pMonster->GetFSM()->ChangeState( WaitInterPolation::GetInstance() );
	}
}


VOID Stiffen::Exit( CMonster* a_pMonster )
{

}