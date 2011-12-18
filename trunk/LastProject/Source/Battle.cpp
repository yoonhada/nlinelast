#include "stdafx.h"

#include "Battle.h"
#include "LockOn.h"
#include "Melee.h"
#include "Spin.h"
#include "RocketPunch.h"
#include "SenpukyakuReady.h"
#include "DashReady.h"
#include "Wait.h"

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

#ifdef _DEBUG
	// Ÿ�� ��ȣ
	static FLOAT target = 0;
	target = static_cast<FLOAT>( a_pMonster->Get_Target() );
	CDebugInterface::GetInstance()->AddMessageFloat( "target", target );
#endif

	// ���� ���ݹ��� �ȿ� �������� �и��� ȸ�� ������ ���� ����
	if( fDistance < 25.0f )
	{
		a_pMonster->GetFSM()->ChangeState( LockOn::GetInstance() );
	}
	// ���Ÿ� ���� ������
	else if( fDistance > 80.0f && fDistance < 120.0f )
	{
		// �߿��θ� �뽬, ������ġ �� ����
		if( a_pMonster->Get_MonsterNumber() == 2 )
		{
			if( FastRand2() < 0.5f )
			{
				CNetwork::GetInstance()->CS_Monster_Attack_Animation( a_pMonster->Get_MonsterNumber(), CMonster::ANIM_DASH );
				a_pMonster->GetFSM()->ChangeState( DashReady::GetInstance() );
			}
			else
			{
				CNetwork::GetInstance()->CS_Monster_Attack_Animation( a_pMonster->Get_MonsterNumber(), CMonster::ANIM_ROCKETPUNCH );
				a_pMonster->GetFSM()->ChangeState( RocketPunch::GetInstance() );
			}
		}
		// �Ǵٸ� �뽬
		else
		{
			CNetwork::GetInstance()->CS_Monster_Attack_Animation( a_pMonster->Get_MonsterNumber(), CMonster::ANIM_DASH );
			a_pMonster->GetFSM()->ChangeState( DashReady::GetInstance() );
		}
	}
	// ���ݹ����� �ƴϸ� ��� ���·�
	else
	{
		a_pMonster->GetFSM()->ChangeState( Wait::GetInstance() );
	}
}


VOID Battle::Exit( CMonster* a_pMonster )
{

}