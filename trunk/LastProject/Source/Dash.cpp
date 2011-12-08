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
	// Dash �ִϸ��̼����� ����
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

	// ���������� Stiffen ���·�
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
	// ��ǥ �޾ƿ���
	D3DXVECTOR3 vPlayerPos	= CObjectManage::GetInstance()->Get_Charactor()[a_pMonster->Get_Target()].Get_CharaPos();
	D3DXVECTOR3 vMonsterPos	= a_pMonster->Get_Pos();

	// �̿���, �뺯
	FLOAT fX = vPlayerPos.x - vMonsterPos.x;
	FLOAT fZ = vPlayerPos.z - vMonsterPos.z;

	// ���� ���Ѵ�.
	static FLOAT fAngle = 0.0f;
	fAngle = D3DXToDegree( (FLOAT)atan( fZ / fX ) );

	// DX ������� ����ϱ� ���� ��ȯ�Ѵ�. 3�ú��� �ð����
	//          |
	// 3��и�  |  4��и�
	//          |
	// |||||||||||||||||||
	//	        |
	// 2��и�  |  1��и�
	//          |

	// 1��и� ( 0��<= Angle < 90��)
	if( vPlayerPos.x > vMonsterPos.x && vPlayerPos.z <= vMonsterPos.z )
	{
		fAngle = abs( fAngle );
	}
	// 2��и� ( 90��<= Angle < 180��)
	else if( vPlayerPos.x <= vMonsterPos.x && vPlayerPos.z < vMonsterPos.z )
	{
		fAngle = 180.0f - fAngle;
	}
	// 3��и� ( 180��<= Angle < 270��)
	else if( vPlayerPos.x < vMonsterPos.x && vPlayerPos.z >= vMonsterPos.z )
	{
		fAngle = abs( fAngle ) + 180.0f;
	}
	// 4��и� ( 270��<= Angle < 360��)
	else if( vPlayerPos.x >= vMonsterPos.x && vPlayerPos.z > vMonsterPos.z )
	{
		fAngle = 360.0f - fAngle;
	}

	FLOAT fMonsterAngle = 0.0f;
	fMonsterAngle = D3DXToDegree( a_pMonster->Get_Angle() );

	// 0���� 90���������� ���� �����Ƿ� -90���ؼ� ������ �����.
	// 0��~ 360�������� ���� �ǵ��� ��ȯ�Ѵ�.
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