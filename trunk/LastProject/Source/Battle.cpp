#include "stdafx.h"

#include "Battle.h"
#include "Seek.h"
#include "LockOn.h"
#include "Melee.h"
#include "Spin.h"
#include "Dash.h"

#include "Charactor.h"
#include "Monster.h"


Battle* Battle::GetInstance()
{
	static Battle Instance;

	return &Instance;
}


VOID Battle::Enter( CMonster* a_pMonster )
{

}


VOID Battle::Execute( CMonster* a_pMonster )
{
	INT Target = -1;
	D3DXVECTOR3 pos;
	FLOAT length = 0.0f;
	static FLOAT min = 0.0f;

	FLOAT fDistance = a_pMonster->Get_TargetDistance();

	// Ÿ�� ��ȣ
	static FLOAT target;
	target = static_cast<FLOAT>( a_pMonster->Get_Target() );
	CDebugInterface::GetInstance()->AddMessageFloat( "target", target );

	// ���� ���ݹ��� �ȿ� �������� �и��� ȸ�� ������ ���� ����
	if( fDistance < 25.0f )
	{
		// ���� ����
		//if( FastRand2() < 0.5f )
		//{
			a_pMonster->GetFSM()->ChangeState( LockOn::GetInstance() );
		//}
		// ȸ�� ����
		/*else
		{
			pMonster->GetFSM()->ChangeState( Spin::GetInstance() );
		}	*/
	}
/*
	// ��� ���� ������
	else if( fDistance > 100.0f && fDistance < 300.0f )
	{
		pMonster->GetFSM()->ChangeState( Dash::GetInstance() );
	}
*/
	// ���ݹ����� �ƴϸ� �ٽ� Ž�� ���·�
	else
	{
		a_pMonster->GetFSM()->ChangeState( Seek::GetInstance() );
	}
}


VOID Battle::Exit( CMonster* a_pMonster )
{

}