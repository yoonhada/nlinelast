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
	// �ӽ�
/*
	// �Լ� �κ�
	FLOAT f = D3DXToDegree( acosf( D3DXVec2Dot( &vMonster, &vPlayer ) / ( D3DXVec2Length( &vMonster ) * D3DXVec2Length( &vPlayer ) ) ) );

	cout << "���� �� : " << f << endl;

	if( f >= 0.0f && f <= 30.0f )
	{
	return TRUE;
	}

	return FALSE;
*/

/*
	D3DXVECTOR2 MonsterPos( 0.0f, 0.0f );
	D3DXVECTOR2 PlayerPos( 10.0f, -10.0f );

	// ������ ���ͷ� ��ȯ�Ѵ�.
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

	// ������ ���� ���ݽþ߿� ������ üũ�Ѵ�.
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
	// ��ǥ �޾ƿ���
	D3DXVECTOR3 vPlayerPos	= CObjectManage::GetInstance()->Get_Charactor()[a_pMonster->Get_Target()].Get_CharaPos();
	D3DXVECTOR3 vMonsterPos	= a_pMonster->Get_Pos();

	// �̿���, �뺯
	FLOAT fX = vPlayerPos.x - vMonsterPos.x;
	FLOAT fZ = vPlayerPos.z - vMonsterPos.z;

	static FLOAT w = 0;

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
		w = 1;
	}
	// 2��и� ( 90��<= Angle < 180��)
	else if( vPlayerPos.x <= vMonsterPos.x && vPlayerPos.z < vMonsterPos.z )
	{
		fAngle = 180.0f - fAngle;
		w = 2;
	}
	// 3��и� ( 180��<= Angle < 270��)
	else if( vPlayerPos.x < vMonsterPos.x && vPlayerPos.z >= vMonsterPos.z )
	{
		fAngle = abs( fAngle ) + 180.0f;
		w = 3;
	}
	// 4��и� ( 270��<= Angle < 360��)
	else if( vPlayerPos.x >= vMonsterPos.x && vPlayerPos.z > vMonsterPos.z )
	{
		fAngle = 360.0f - fAngle;
		w = 4;
	}

	static FLOAT fMonsterAngle = 0.0f;
#ifdef _DEBUG
	// ���� ���� �÷��̾� �� ( +x���� 0��, CW�� +����)
	CDebugInterface::GetInstance()->AddMessageFloat( "Player_Angle", fAngle );

	static FLOAT Posx = 0.0f;
	static FLOAT Posz = 0.0f;
	Posx = vPlayerPos.x;
	Posz = vPlayerPos.z;
	CDebugInterface::GetInstance()->AddMessageFloat( "Player_x", Posx );
	CDebugInterface::GetInstance()->AddMessageFloat( "Player_z", Posz );
	CDebugInterface::GetInstance()->AddMessageFloat( "w", w );

	// ���� �� ( +x���� 0��, CW�� +���� )
	
	fMonsterAngle = D3DXToDegree( a_pMonster->Get_Angle() );
	CDebugInterface::GetInstance()->AddMessageFloat( "Monster_Angle", fMonsterAngle );
#endif

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

	// ���� �������� ��������
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