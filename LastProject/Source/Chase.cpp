#include "stdafx.h"

#include "Chase.h"
#include "Charactor.h"
#include "Monster.h"


Chase* Chase::GetInstance()
{
	static Chase Instance;

	return &Instance;
}


VOID Chase::Enter( CMonster* pMonster )
{

}


VOID Chase::Execute( CMonster* pMonster )
{

}


VOID Chase::Exit( CMonster* pMonster )
{

}