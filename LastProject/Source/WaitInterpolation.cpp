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
	// 애니메이션 보간이 끝나면 다시 탐색 상태로
	if( a_pMonster->Get_ChangingAnimation() == FALSE )
	{
		// 호스트이면 상태를 바꾼다.
		if( CObjectManage::GetInstance()->IsHost() == TRUE )
		{
			a_pMonster->GetFSM()->ChangeState( Seek::GetInstance() );

		}
		else
		{
			a_pMonster->GetFSM()->ChangeState( NULL );
		}
	}
}


VOID WaitInterPolation::Exit( CMonster* a_pMonster )
{

}