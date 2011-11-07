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
	// ��ǥ �޾ƿ���
	D3DXVECTOR3 vPlayerPos	= CMainManage::GetInstance()->Get_CharactorList()[pMonster->Get_Target()]->Get_CharaPos();
	D3DXVECTOR3 vMonsterPos	= pMonster->Get_Pos();

	// �̿���
	FLOAT x = vPlayerPos.x - vMonsterPos.x;
	// �뺯
	FLOAT z = vPlayerPos.z - vMonsterPos.z;
	
//	if( x == 0.0f )		x = 1.0f;
//	if( z == 0.0f )		z = 1.0f;

	static FLOAT w = 0;

	// ���� ���Ѵ�.
	static FLOAT fAngle = 0.0f;
	fAngle = (FLOAT)atan( z / x ) * 180.0f / D3DX_PI;

	// 1��и鿡 �ִ� ���� ��
	if( vPlayerPos.z > vMonsterPos.z && vPlayerPos.x > vMonsterPos.x )
	{
		fAngle = fAngle;
		w = 1;
	}
	// 2, 3��и鿡 �ִ� ���� ��
	else if( ( vPlayerPos.z > vMonsterPos.z && vPlayerPos.x < vMonsterPos.x ) ||
			 ( vPlayerPos.z < vMonsterPos.z && vPlayerPos.x < vMonsterPos.x ) )
	{
		fAngle += 180.0f;
		w = 23;
	}
	// 4��и�
	else
	{
		fAngle = 360.0f + fAngle;
		w = 4;
	}

	// ���� ���� �÷��̾� �� ( +x���� 0��, CCW�� +����)
	CDebugInterface::GetInstance()->AddMessageFloat( "Player_Angle", fAngle );

	static FLOAT Posx = 0.0f;
	static FLOAT Posz = 0.0f;
	Posx = vPlayerPos.x;
	Posz = vPlayerPos.z;
	CDebugInterface::GetInstance()->AddMessageFloat( "Player_x", Posx );
	CDebugInterface::GetInstance()->AddMessageFloat( "Player_z", Posz );
	CDebugInterface::GetInstance()->AddMessageFloat( "w", w );

	// ���� �� ( +x���� 0��, CW�� +���� )
	static FLOAT fMonsterAngle = 0.0f;
	fMonsterAngle = pMonster->Get_Angle() * 180.0f / D3DX_PI;
	CDebugInterface::GetInstance()->AddMessageFloat( "Monster_Angle", fMonsterAngle );

	static D3DXVECTOR3 vRot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	//vRot.y
	if( fMonsterAngle != fAngle )
	{
		pMonster->Set_Angle( D3DXToRadian( 270.0f - fAngle )  );
//		D3DXVec2Lerp(
	}

	// ���� �������� ��������
	if( 1 )
	{
		pMonster->GetFSM()->ChangeState( Melee::GetInstance() );
	}

}


VOID LockOn::Exit( CMonster* pMonster )
{

}