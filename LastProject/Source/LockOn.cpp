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
	// 임시
/*
	// 함수 부분
	FLOAT f = D3DXToDegree( acosf( D3DXVec2Dot( &vMonster, &vPlayer ) / ( D3DXVec2Length( &vMonster ) * D3DXVec2Length( &vPlayer ) ) ) );

	cout << "각도 차 : " << f << endl;

	if( f >= 0.0f && f <= 30.0f )
	{
	return TRUE;
	}

	return FALSE;
*/

/*
	D3DXVECTOR2 MonsterPos( 0.0f, 0.0f );
	D3DXVECTOR2 PlayerPos( 10.0f, -10.0f );

	// 각도를 벡터로 변환한다.
	FLOAT MonsterAngle = 0.0f;
	FLOAT a = cos( D3DXToRadian( MonsterAngle ) );
	FLOAT b = sin( D3DXToRadian( MonsterAngle ) );

	D3DXVECTOR2 vMonster( a, b );
	D3DXVECTOR2 vPlayer( PlayerPos.x - MonsterPos.x, PlayerPos.y - MonsterPos.y );

	D3DXVec2Normalize( &vMonster, &vMonster );
	D3DXVec2Normalize( &vPlayer, &vPlayer );

	cout << "Monster Vector : " << vMonster.x << ", " << vMonster.y << endl;
	cout << "Player  Vector : " << vPlayer.x << ", " << vPlayer.y << endl;

	cout << D3DXToDegree( acosf( D3DXVec2Dot( &vMonster, &vPlayer ) ) ) << endl;

	// 내적을 구해 공격시야에 들어온지 체크한다.
	if( isInSight( vMonster, vPlayer ) )
	{
		cout << "In" << endl;
	}
	else
	{
		cout << "Out" << endl;
	}
*/
}

VOID LockOn::Execute( CMonster* a_pMonster )
{
	// 좌표 받아오기
	D3DXVECTOR3 vPlayerPos	= CObjectManage::GetInstance()->Get_Charactor()[a_pMonster->Get_Target()].Get_CharaPos();
	D3DXVECTOR3 vMonsterPos	= a_pMonster->Get_Pos();

	// 이웃변, 대변
	FLOAT fX = vPlayerPos.x - vMonsterPos.x;
	FLOAT fZ = vPlayerPos.z - vMonsterPos.z;

	static FLOAT w = 0;

	// 각을 구한다.
	static FLOAT fAngle = 0.0f;
	fAngle = D3DXToDegree( (FLOAT)atan( fZ / fX ) );

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

	static FLOAT fMonsterAngle = 0.0f;
#ifdef _DEBUG
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
	
	fMonsterAngle = D3DXToDegree( a_pMonster->Get_Angle() );
	CDebugInterface::GetInstance()->AddMessageFloat( "Monster_Angle", fMonsterAngle );
#endif

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
		CNetwork::GetInstance()->CS_Monster_LockOn( 0, f );

		a_pMonster->Set_Angle( D3DXToRadian( f )  );
	}

	// 공격 각도내에 들어왔으면
	if( isInSight() )
	{
		CNetwork::GetInstance()->CS_Monster_Attack_Animation( 0, 1 );

		a_pMonster->GetFSM()->ChangeState( Melee::GetInstance() );
	}
}


VOID LockOn::Exit( CMonster* a_pMonster )
{

}


BOOL LockOn::isInSight()
{
	
	return TRUE;
}