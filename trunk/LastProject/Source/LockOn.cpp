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


VOID LockOn::Enter( CMonster* pMonster )
{

}


VOID LockOn::Execute( CMonster* pMonster )
{
	// 좌표 받아오기
	D3DXVECTOR3 vPlayerPos	= CMainManage::GetInstance()->Get_CharactorList()[pMonster->Get_Target()]->Get_CharaPos();
	D3DXVECTOR3 vMonsterPos	= pMonster->Get_Pos();

	// 이웃변
	FLOAT x = vPlayerPos.x - vMonsterPos.x;
	// 대변
	FLOAT z = vPlayerPos.z - vMonsterPos.z;
	
//	if( x == 0.0f )		x = 1.0f;
//	if( z == 0.0f )		z = 1.0f;

	// 각을 구한다.
	static FLOAT fAngle = 0.0f;
	fAngle = (FLOAT)atan( z / x ) * 180.0f / 3.14156f;

	// 1사분면에 있는 각일 때
	if( vPlayerPos.z > vMonsterPos.z && vPlayerPos.x > vMonsterPos.x )
	{
		fAngle = fAngle;
	}
	// 2, 3사분면에 있는 각일 때
	else if( ( vPlayerPos.z > vMonsterPos.z && vPlayerPos.x < vMonsterPos.x ) ||
			 ( vPlayerPos.z < vMonsterPos.z && vPlayerPos.x < vMonsterPos.x ) )
	{
		fAngle += 180.0f;
	}
	// 4사분면
	else
	{
		fAngle += 360.0f;
	}

	// 몬스터 기준 플레이어 각 ( +x축이 0도, CCW가 +각도)
	CDebugInterface::GetInstance()->AddMessageFloat( "angle", fAngle );

	// 몬스터 각 ( +x축이 0도, CW가 +각도 )
	static FLOAT fMonsterAngle = 0.0f;
	fMonsterAngle = pMonster->Get_Angle() * 180.0f / 3.14156f;
	CDebugInterface::GetInstance()->AddMessageFloat( "MonsterAngle", fMonsterAngle );

	static D3DXVECTOR3 vRot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	//vRot.y
	if( fMonsterAngle != fAngle )
	{
		pMonster->Set_Angle( D3DXToRadian( 270.0f - fAngle )  );
	}

	// 공격 각도내에 들어왔으면
//	if( 1 )
//	{
		pMonster->GetFSM()->ChangeState( Melee::GetInstance() );
//	}
}


VOID LockOn::Exit( CMonster* pMonster )
{

}