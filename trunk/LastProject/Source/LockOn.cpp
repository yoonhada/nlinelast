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

	// ���� ���Ѵ�.
	static FLOAT fAngle = 0.0f;
	fAngle = (FLOAT)atan( z / x ) * 180.0f / 3.14156f;

	// 1��и鿡 �ִ� ���� ��
	if( vPlayerPos.z > vMonsterPos.z && vPlayerPos.x > vMonsterPos.x )
	{
		fAngle = fAngle;
	}
	// 2, 3��и鿡 �ִ� ���� ��
	else if( ( vPlayerPos.z > vMonsterPos.z && vPlayerPos.x < vMonsterPos.x ) ||
			 ( vPlayerPos.z < vMonsterPos.z && vPlayerPos.x < vMonsterPos.x ) )
	{
		fAngle += 180.0f;
	}
	// 4��и�
	else
	{
		fAngle += 360.0f;
	}

	// ���� ���� �÷��̾� �� ( +x���� 0��, CCW�� +����)
	CDebugInterface::GetInstance()->AddMessageFloat( "angle", fAngle );

	// ���� �� ( +x���� 0��, CW�� +���� )
	static FLOAT fMonsterAngle = 0.0f;
	fMonsterAngle = pMonster->Get_Angle() * 180.0f / 3.14156f;
	CDebugInterface::GetInstance()->AddMessageFloat( "MonsterAngle", fMonsterAngle );

	static D3DXVECTOR3 vRot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	//vRot.y
	if( fMonsterAngle != fAngle )
	{
		pMonster->Set_Angle( D3DXToRadian( 270.0f - fAngle )  );
	}

	// ���� �������� ��������
//	if( 1 )
//	{
		pMonster->GetFSM()->ChangeState( Melee::GetInstance() );
//	}
}


VOID LockOn::Exit( CMonster* pMonster )
{

}