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

	m_vPos		= a_pMonster->Get_Pos();
	m_vNextPos	= a_pMonster->Get_TargetPos();
	m_fT		= a_pMonster->Get_TargetDistance() / 10.0f * 0.25f;
/*
	CDebugConsole::GetInstance()->Messagef( L"Pos     : %f %f \n", m_vPos.x, m_vPos.z );
	CDebugConsole::GetInstance()->Messagef( L"NextPos : %f %f \n", m_vNextPos.x, m_vNextPos.z );
	CDebugConsole::GetInstance()->Messagef( L"Distance: %f \n", a_pMonster->Get_TargetDistance() );
	CDebugConsole::GetInstance()->Messagef( L"Time    : %f \n", m_fT );
*/
	SetAngle( a_pMonster );
}


VOID Dash::Execute( CMonster* a_pMonster )
{
	static FLOAT t = 0.0f;
	t += CFrequency::GetInstance()->getFrametime();

	// ���������� Stiffen ���·�
	if( t >= m_fT )
	{
		t = 0.0f;

		a_pMonster->GetFSM()->ChangeState( Sliding::GetInstance() );
//		CDebugConsole::GetInstance()->Messagef( L"ok \n" );
	}
	else
	{
		D3DXVECTOR3 vPos( 0.0f, 0.0f, 0.0f );

		D3DXVec3Lerp( &vPos, &m_vPos, &m_vNextPos, t / m_fT );
		a_pMonster->Set_Pos( vPos );
	}
}


VOID Dash::Exit( CMonster* a_pMonster )
{

}


VOID Dash::SetAngle( CMonster* a_pMonster )
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
		CNetwork::GetInstance()->CS_Monster_LockOn( 0, f );

		a_pMonster->Set_Angle( D3DXToRadian( f )  );
	}
}