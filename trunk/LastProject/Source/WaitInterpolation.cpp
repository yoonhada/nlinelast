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
	// 애니메이션 보간이 끝나면 다시 탐색 상태로
	if( pMonster->Get_ChangingAnimation() == FALSE )
	{
		pMonster->GetFSM()->ChangeState( Seek::GetInstance() );
	}
}


VOID WaitInterPolation::Exit( CMonster* pMonster )
{

}