#include "stdafx.h"

#include "Dash.h"
#include "Stiffen.h"
#include "Sliding.h"
#include "Charactor.h"


Dash* Dash::GetInstance()
{
	static Dash Instance;

	return &Instance;
}


VOID Dash::Enter( CMonster* a_pMonster )
{
	// Dash 애니메이션으로 변경
	a_pMonster->ChangeAnimation( CMonster::ANIM_DASH );
	CDebugConsole::GetInstance()->Messagef( L"Dash : ANIM_DASH \n" );
/*
	m_vPos		= a_pMonster->Get_Pos();
	m_vNextPos	= a_pMonster->Get_TargetPos();
*/
	a_pMonster->Set_DashData( a_pMonster->Get_Pos(), a_pMonster->Get_TargetPos() );
	a_pMonster->Set_InterpolationTime( a_pMonster->Get_TargetDistance() / 10.0f * 0.15f );

	if( CObjectManage::GetInstance()->IsHost() == TRUE )
	{
		FLOAT fAngle = SetAngle( a_pMonster );
		CNetwork::GetInstance()->CS_Monster_Attack_Animation2( a_pMonster->Get_MonsterNumber(), CMonster::ANIM_DASH, fAngle, a_pMonster->Get_Pos(), a_pMonster->Get_TargetPos(), a_pMonster->Get_TargetDistance() );
	}
}


VOID Dash::Execute( CMonster* a_pMonster )
{
	a_pMonster->Set_UpdateTime();

	// 도착했으면 Stiffen 상태로
	FLOAT t = a_pMonster->Get_Time();
	if( t >= a_pMonster->Get_InterpolationTime() )
	{
		a_pMonster->Set_ClearTime();

		a_pMonster->GetFSM()->ChangeState( Sliding::GetInstance() );
	}
	else
	{
		D3DXVECTOR3 vPos( 0.0f, 0.0f, 0.0f );

		D3DXVec3Lerp( &vPos, &a_pMonster->Get_DashStartPos(), &a_pMonster->Get_DashEndPos(), t / a_pMonster->Get_InterpolationTime() );
		a_pMonster->Set_Pos( vPos );
	}
}


VOID Dash::Exit( CMonster* a_pMonster )
{

}


FLOAT Dash::SetAngle( CMonster* a_pMonster )
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