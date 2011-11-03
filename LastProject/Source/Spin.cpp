#include "stdafx.h"

#include "Spin.h"
#include "Stiffen.h"

#include "Monster.h"


Spin* Spin::GetInstance()
{
	static Spin Instance;

	return &Instance;
}


VOID Spin::Enter( CMonster* pMonster )
{

}


VOID Spin::Execute( CMonster* pMonster )
{
	pMonster->ChangeAnimation( 2 );

	pMonster->GetFSM()->ChangeState( Stiffen::GetInstance() );
}


VOID Spin::Exit( CMonster* pMonster )
{

}