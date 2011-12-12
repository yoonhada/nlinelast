#include "stdafx.h"

#include "DashReady.h"
#include "Dash.h"
#include "Monster.h"
#include "Charactor.h"


DashReady* DashReady::GetInstance()
{
	static DashReady Instance;

	return &Instance;
}


VOID DashReady::Enter( CMonster* a_pMonster )
{
	// Dash �ִϸ��̼����� ����
	a_pMonster->ChangeAnimation( CMonster::ANIM_DASH );
#ifdef _DEBUG
	CDebugConsole::GetInstance()->Messagef( L"DashReady : ANIM_DASH \n" );
#endif

	if( CObjectManage::GetInstance()->IsHost() == TRUE )
	{
		FLOAT fAngle = SetAngle( a_pMonster );
	}
}


VOID DashReady::Execute( CMonster* a_pMonster )
{
	// �ִϸ��̼� ������ ������ �ٽ� Ž�� ���·�
	if( a_pMonster->Get_ChangingAnimation() == FALSE )
	{
		a_pMonster->GetFSM()->ChangeState( Dash::GetInstance() );
	}
}


VOID DashReady::Exit( CMonster* a_pMonster )
{

}


FLOAT DashReady::SetAngle( CMonster* a_pMonster )
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