#include "stdafx.h"

#include "Seek.h"
#include "Battle.h"
#include "Chase.h"

#include "Charactor.h"
#include "Monster.h"


Seek* Seek::GetInstance()
{
	static Seek Instance;

	return &Instance;
}


VOID Seek::Enter( CMonster* pMonster )
{

}


VOID Seek::Execute( CMonster* pMonster )
{
	INT Target = -1;
	D3DXVECTOR3 pos;
	FLOAT length = 0.0f;
	static FLOAT min = 0.0f;

	// 몬스터와 가장 가까이에 있는 유저를 찾는다.
	for( INT i=0; i<4; ++i )
	{
		if( CMainManage::GetInstance()->Get_CharactorList()[i]->Get_Active() )
		{
			// 유저 위치 - 보스 몬스터
			pos = CMainManage::GetInstance()->Get_CharactorList()[i]->Get_CharaPos() - pMonster->Get_Pos();

			// 거리 구하기
			length = D3DXVec3Length( &pos );

			if( i == 0 )
			{
				min = length;
				Target = i;
			}
			else
			{
				// 더 가까이 있으면
				if( length < min )
				{
					min = length;
					Target = i;
				}
			}
		}
	}

	// 가장 가까이에 있는 유저가 공격 범위에 있으면 전투 상태로 전환
	if( min < 100.0f )
	{
		pMonster->Set_Target( Target );
		pMonster->Set_TargetDistance( min );
		pMonster->GetFSM()->ChangeState( Battle::GetInstance() );
	}
	// 범위에 없으면 가장 가까운 유저 추격
	else if( min >= 100.0f && min <= 150.0f )
	{
		pMonster->Set_Target( Target );
		D3DXVECTOR3 UnitVector = CMainManage::GetInstance()->Get_CharactorList()[Target]->Get_CharaPos() - pMonster->Get_Pos();
		D3DXVec3Normalize( &UnitVector, &UnitVector );
		pMonster->Set_TargetUnitVector( UnitVector );
		pMonster->Set_TargetPos( CMainManage::GetInstance()->Get_CharactorList()[Target]->Get_CharaPos() );
		pMonster->GetFSM()->ChangeState( Chase::GetInstance() );
	}
	else
	{
//		pMonster->ChangeAnimation( 0 );
	}

	CDebugInterface::GetInstance()->AddMessageFloat( "length", min );
}


VOID Seek::Exit( CMonster* pMonster )
{

}