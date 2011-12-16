#include "stdafx.h"

#include "RocketPunch.h"
#include "Stiffen.h"

#include "Monster.h"
#include "Charactor.h"


RocketPunch* RocketPunch::GetInstance()
{
	static RocketPunch Instance;

	return &Instance;
}


VOID RocketPunch::Enter( CMonster* a_pMonster )
{
	SetAngle( a_pMonster );



	// Melee 공격 애니메이션으로 바꾼다.
	a_pMonster->ChangeAnimation( CMonster::ANIM_ROCKETPUNCH );
#ifdef _DEBUG
	CDebugConsole::GetInstance()->Messagef( L"RocketPunch : ANIM_ROCKETPUNCH \n" );
#endif

	if( a_pMonster->Get_MonsterNumber() == 2 )
	{
		CSound::GetInstance()->PlayEffect( 0 );	
	}
	else
	{
		CSound::GetInstance()->PlayEffect( 0 );	
	}
}


VOID RocketPunch::Execute( CMonster* a_pMonster )
{
	if( a_pMonster->Get_AnimationEndCheck() == FALSE )
	{
		// 공격 충돌 박스 생성
		a_pMonster->CreateAttackBoundBox();
		
		// 경직 상태로
		a_pMonster->GetFSM()->ChangeState( Stiffen::GetInstance() );
	}
}


VOID RocketPunch::Exit( CMonster* a_pMonster )
{

}


FLOAT RocketPunch::SetAngle( CMonster* a_pMonster )
{
	// 좌표 받아오기
	D3DXVECTOR3 vPlayerPos	= CObjectManage::GetInstance()->Get_Charactor()[a_pMonster->Get_Target()].Get_CharaPos();
	D3DXVECTOR3 vMonsterPos	= a_pMonster->Get_Pos();

	// 이웃변, 대변
	FLOAT fX = vPlayerPos.x - vMonsterPos.x;
	FLOAT fZ = vPlayerPos.z - vMonsterPos.z;

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
	}
	// 2사분면 ( 90˚<= Angle < 180˚)
	else if( vPlayerPos.x <= vMonsterPos.x && vPlayerPos.z < vMonsterPos.z )
	{
		fAngle = 180.0f - fAngle;
	}
	// 3사분면 ( 180˚<= Angle < 270˚)
	else if( vPlayerPos.x < vMonsterPos.x && vPlayerPos.z >= vMonsterPos.z )
	{
		fAngle = abs( fAngle ) + 180.0f;
	}
	// 4사분면 ( 270˚<= Angle < 360˚)
	else if( vPlayerPos.x >= vMonsterPos.x && vPlayerPos.z > vMonsterPos.z )
	{
		fAngle = 360.0f - fAngle;
	}

	FLOAT fMonsterAngle = 0.0f;
	fMonsterAngle = D3DXToDegree( a_pMonster->Get_Angle() );

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
		CNetwork::GetInstance()->CS_Monster_LockOn( a_pMonster->Get_MonsterNumber(), f );

		a_pMonster->Set_Angle( D3DXToRadian( f )  );
	}

	return f;
}