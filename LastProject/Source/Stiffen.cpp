#include "stdafx.h"

#include "Stiffen.h"
#include "WaitInterPolation.h"

#include "Monster.h"
#include "Frequency.h"


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
	a_pMonster->UpdateTime();

	if( a_pMonster->Get_Time() >= 1.0f )
	{
		a_pMonster->ClearTime();

		// 대기 애니메이션으로 바꾼다.
		a_pMonster->ChangeAnimation( CMonster::ANIM_STAND );

		// 경직상태가 끝나면 애니메이션 보간을 위해 보간 대기 상태로
		a_pMonster->GetFSM()->ChangeState( WaitInterPolation::GetInstance() );
	}
}


VOID Stiffen::Exit( CMonster* a_pMonster )
{

}