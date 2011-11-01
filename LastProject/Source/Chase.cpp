#include "stdafx.h"

#include "Chase.h"


Chase& Chase::GetInstance()
{
	static Chase Instance;

	return &Instance;
}


VOID Chase::Enter( Miner* pMiner )
{

}


VOID Chase::Execute( Miner* pMiner )
{

}


VOID Chase::Exit( Miner* pMiner )
{

}