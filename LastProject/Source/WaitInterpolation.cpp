#include "stdafx.h"

#include "WaitInterPolation.h"
#include "Seek.h"

#include "Monster.h"


WaitInterPolation* WaitInterPolation::GetInstance()
{
	static WaitInterPolation Instance;

	return &Instance;
}


VOID WaitInterPolation::Enter( CMonster* a_pMonster )
{

}


VOID WaitInterPolation::Execute( CMonster* a_pMonster )
{
	// �ִϸ��̼� ������ ������ �ٽ� Ž�� ���·�
	if( a_pMonster->Get_ChangingAnimation() == FALSE )
	{
		a_pMonster->GetFSM()->ChangeState( Seek::GetInstance() );
	}
}


VOID WaitInterPolation::Exit( CMonster* a_pMonster )
{

}