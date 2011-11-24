#include "stdafx.h"

#include "LockOn.h"
#include "Melee.h"

#include "Charactor.h"
#include "Monster.h"


LockOn* LockOn::GetInstance()
{
	static LockOn Instance;

	return &Instance;
}


VOID LockOn::Enter( CMonster* a_pMonster )
{

}


VOID LockOn::Execute( CMonster* a_pMonster )
{
	// 좌표 받아오기
	D3DXVECTOR3 vPlayerPos	= CObjectManage::GetInstance()->Get_CharactorList()[a_pMonster->Get_Target()]->Get_CharaPos();
	D3DXVECTOR3 vMonsterPos	= a_pMonster->Get_Pos();

	// 이웃변
	FLOAT x = vPlayerPos.x - vMonsterPos.x;
	// 대변
	FLOAT z = vPlayerPos.z - vMonsterPos.z;
	
//	if( x == 0.0f )		x = 1.0f;
//	if( z == 0.0f )		z = 1.0f;

	static FLOAT w = 0;

	// 각을 구한다.
	static FLOAT fAngle = 0.0f;
	fAngle = (FLOAT)atan( z / x ) * 180.0f / D3DX_PI;

	//          |
	// 3사분면  |  4사분면
	//          |
	// |||||||||||||||||||
	//	        |
	// 2사분면  |  1사분면
	//          |

	// 4사분면에 있는 각일 때
	if( vPlayerPos.z > vMonsterPos.z && vPlayerPos.x > vMonsterPos.x )
	{
		fAngle = fAngle;
		w = 4;
	}
	// 2, 3사분면에 있는 각일 때
	else if( ( vPlayerPos.z > vMonsterPos.z && vPlayerPos.x < vMonsterPos.x ) ||
		( vPlayerPos.z < vMonsterPos.z && vPlayerPos.x < vMonsterPos.x ) )
	{
		fAngle += 180.0f;
		w = 23;
	}
	// 1사분면
	else
	{
		fAngle = 360.0f + fAngle;
		w = 1;
	}

	// 몬스터 기준 플레이어 각 ( +x축이 0도, CCW가 +각도)
	CDebugInterface::GetInstance()->AddMessageFloat( "Player_Angle", fAngle );

	static FLOAT Posx = 0.0f;
	static FLOAT Posz = 0.0f;
	Posx = vPlayerPos.x;
	Posz = vPlayerPos.z;
	CDebugInterface::GetInstance()->AddMessageFloat( "Player_x", Posx );
	CDebugInterface::GetInstance()->AddMessageFloat( "Player_z", Posz );
	CDebugInterface::GetInstance()->AddMessageFloat( "w", w );

	// 몬스터 각 ( +x축이 0도, CW가 +각도 )
	static FLOAT fMonsterAngle = 0.0f;
	fMonsterAngle = a_pMonster->Get_Angle() * 180.0f / D3DX_PI;
	CDebugInterface::GetInstance()->AddMessageFloat( "Monster_Angle", fMonsterAngle );

	static D3DXVECTOR3 vRot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	if( fMonsterAngle != fAngle )
	{
		a_pMonster->Set_Angle( D3DXToRadian( 270.0f - fAngle )  );
	}
	
	// 공격 각도내에 들어왔으면
	if( isInSight() )
	{
		a_pMonster->GetFSM()->ChangeState( Melee::GetInstance() );
		a_pMonster->GetFSM()->GetCurrentState()->Enter( a_pMonster );
	}

}


VOID LockOn::Exit( CMonster* a_pMonster )
{

}


BOOL LockOn::isInSight()
{
	return TRUE;
}