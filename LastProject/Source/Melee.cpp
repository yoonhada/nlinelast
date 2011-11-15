#include "stdafx.h"

#include "Melee.h"
#include "Stiffen.h"

#include "Monster.h"


Melee* Melee::GetInstance()
{
	static Melee Instance;

	return &Instance;
}


VOID Melee::Enter( CMonster* a_pMonster )
{

}


VOID Melee::Execute( CMonster* a_pMonster )
{
	a_pMonster->ChangeAnimation( 2 );

	a_pMonster->GetFSM()->ChangeState( Stiffen::GetInstance() );
}


VOID Melee::Exit( CMonster* a_pMonster )
{

}