#include "stdafx.h"

#include "WaitInterPolation.h"
#include "Seek.h"

#include "Monster.h"


WaitInterPolation* WaitInterPolation::GetInstance()
{
	static WaitInterPolation Instance;

	return &Instance;
}


VOID WaitInterPolation::Enter( CMonster* pMonster )
{

}


VOID WaitInterPolation::Execute( CMonster* pMonster )
{
	// �ִϸ��̼� ������ ������ �ٽ� Ž�� ���·�
	if( pMonster->Get_ChangingAnimation() == FALSE )
	{
		pMonster->GetFSM()->ChangeState( Seek::GetInstance() );
	}
}


VOID WaitInterPolation::Exit( CMonster* pMonster )
{

}