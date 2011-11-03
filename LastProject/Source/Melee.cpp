#include "stdafx.h"

#include "Melee.h"
#include "Stiffen.h"

#include "Monster.h"


Melee* Melee::GetInstance()
{
	static Melee Instance;

	return &Instance;
}


VOID Melee::Enter( CMonster* pMonster )
{

}


VOID Melee::Execute( CMonster* pMonster )
{
	pMonster->ChangeAnimation( 2 );

	pMonster->GetFSM()->ChangeState( Stiffen::GetInstance() );
}


VOID Melee::Exit( CMonster* pMonster )
{

}