#include "stdafx.h"

#include "Combo2Stiffen.h"
#include "COmbo3.h"

#include "Monster.h"


Combo2Stiffen* Combo2Stiffen::GetInstance()
{
	static Combo2Stiffen Instance;

	return &Instance;
}


VOID Combo2Stiffen::Enter( CMonster* a_pMonster )
{

}


VOID Combo2Stiffen::Execute( CMonster* a_pMonster )
{
	a_pMonster->Set_UpdateTime();

	if( a_pMonster->Get_Time() >= 2.0f )
	{
		a_pMonster->Set_ClearTime();

		// 대기 애니메이션으로 바꾼다.
//		a_pMonster->ChangeAnimation( CMonster::ANIM_STAND );

		// 경직상태가 끝나면 애니메이션 보간을 위해 보간 대기 상태로
		a_pMonster->GetFSM()->ChangeState( Combo3::GetInstance() );
	}
}


VOID Combo2Stiffen::Exit( CMonster* a_pMonster )
{

}