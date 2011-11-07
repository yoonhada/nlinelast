#include "stdafx.h"

#include "Chase.h"
#include "Seek.h"

#include "Frequency.h"
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
	if( (INT)pMonster->Get_Pos().x != (INT)pMonster->Get_TargetPos().x && 
		(INT)pMonster->Get_Pos().z != (INT)pMonster->Get_TargetPos().z )
	{
		D3DXVECTOR3 pos( 0.0f, 0.0f, 0.0f );
		pos.x = pMonster->Get_Pos().x + pMonster->Get_TargetUnitVector().x * 20.0f * CFrequency::GetInstance()->getFrametime();
		pos.z = pMonster->Get_Pos().z + pMonster->Get_TargetUnitVector().z * 20.0f * CFrequency::GetInstance()->getFrametime();

		pMonster->Set_Pos( pos );
	}
	else
	{
		pMonster->GetFSM()->ChangeState( Seek::GetInstance() );
	}
}


VOID Chase::Exit( CMonster* pMonster )
{

}