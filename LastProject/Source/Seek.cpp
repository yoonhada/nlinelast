#include "stdafx.h"

#include "Seek.h"
#include "Battle.h"

#include "Charactor.h"
#include "Monster.h"


Seek* Seek::GetInstance()
{
	static Seek Instance;

	return &Instance;
}


VOID Seek::Enter( CMonster* pMonster )
{

}


VOID Seek::Execute( CMonster* pMonster )
{
	INT Target = -1;
	D3DXVECTOR3 pos;
	FLOAT length = 0.0f;
	static FLOAT min = 0.0f;

	// ���Ϳ� ���� �����̿� �ִ� ������ ã�´�.
	for( INT i=0; i<4; ++i )
	{
		if( CMainManage::GetInstance()->Get_CharactorList()[i]->Get_Active() )
		{
			// ���� ��ġ - ���� ����
			pos = CMainManage::GetInstance()->Get_CharactorList()[i]->Get_CharaPos() - pMonster->Get_Pos();

			// �Ÿ� ���ϱ�
			length = D3DXVec3Length( &pos );

			if( i == 0 )
			{
				min = length;
				Target = i;
			}
			else
			{
				// �� ������ ������
				if( length < min )
				{
					min = length;
					Target = i;
				}
			}
		}
	}

	// ���� �����̿� �ִ� ������ ���� ������ ������ ���� ���·� ��ȯ
	if( min < 500.0f )
	{
		pMonster->Set_Target( Target );
		pMonster->Set_TargetDistance( min );
		pMonster->GetFSM()->ChangeState( Battle::GetInstance() );
	}
	// ������ ������ ���� ����� ���� �߰�
	else
	{
//		pMonster->Set_Target( -1 );
//		pMonster->ChangeState( Chase::GetInstance() );
		pMonster->ChangeAnimation( 0 );
	}

	CDebugInterface::GetInstance()->AddMessageFloat( "length", min );
}


VOID Seek::Exit( CMonster* pMonster )
{

}