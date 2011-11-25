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
	fAngle = D3DXToDegree( (FLOAT)atan( z / x ) );

	// DX 방식으로 계산하기 위해 변환한다. 3시부터 시계방향
	//          |
	// 3사분면  |  4사분면
	//          |
	// |||||||||||||||||||
	//	        |
	// 2사분면  |  1사분면
	//          |

	// 1사분면 ( 0˚<= Angle < 90˚)
	if( vPlayerPos.x > vMonsterPos.x && vPlayerPos.z <= vMonsterPos.z )
	{
		fAngle = abs( fAngle );
		w = 1;
	}
	// 2사분면 ( 90˚<= Angle < 180˚)
	else if( vPlayerPos.x <= vMonsterPos.x && vPlayerPos.z < vMonsterPos.z )
	{
		fAngle = 180.0f - fAngle;
		w = 2;
	}
	// 3사분면 ( 180˚<= Angle < 270˚)
	else if( vPlayerPos.x < vMonsterPos.x && vPlayerPos.z >= vMonsterPos.z )
	{
		fAngle = abs( fAngle ) + 180.0f;
		w = 3;
	}
	// 4사분면 ( 270˚<= Angle < 360˚)
	else if( vPlayerPos.x >= vMonsterPos.x && vPlayerPos.z > vMonsterPos.z )
	{
		fAngle = 360.0f - fAngle;
		w = 4;
	}

	// 몬스터 기준 플레이어 각 ( +x축이 0도, CW가 +각도)
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
	fMonsterAngle = D3DXToDegree( a_pMonster->Get_Angle() );
	CDebugInterface::GetInstance()->AddMessageFloat( "Monster_Angle", fMonsterAngle );

	// 0˚가 90˚방향으로 보고 있으므로 -90˚해서 방향을 맞춘다.
	// 0˚~ 360˚사이의 값이 되도록 변환한다.
	FLOAT f = fAngle;
	if( f < 90.0f )
	{
		f = 270.0f + f;
	}
	else
	{
		f = f - 90.0f;
	}

	if( fMonsterAngle != fAngle )
	{	
		a_pMonster->Set_Angle( D3DXToRadian( f )  );
	}

	// 공격 각도내에 들어왔으면
	if( isInSight() )
	{
//		a_pMonster->GetFSM()->ChangeState( Melee::GetInstance() );
//		a_pMonster->GetFSM()->GetCurrentState()->Enter( a_pMonster );
	}
}


VOID LockOn::Exit( CMonster* a_pMonster )
{

}


BOOL LockOn::isInSight()
{
	
	return TRUE;
}