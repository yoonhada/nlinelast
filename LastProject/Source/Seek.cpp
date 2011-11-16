#include "stdafx.h"

#include "Seek.h"
#include "Battle.h"
#include "Chase.h"

#include "Charactor.h"
#include "Monster.h"


Seek* Seek::GetInstance()
{
	static Seek Instance;

	return &Instance;
}


VOID Seek::Enter( CMonster* a_pMonster )
{

}


VOID Seek::Execute( CMonster* a_pMonster )
{
	INT Target = -1;
	D3DXVECTOR3 pos;
	FLOAT length = 0.0f;
	static FLOAT min = 0.0f;

	// ���Ϳ� ���� �����̿� �ִ� ������ ã�´�.
	for( INT i=0; i<4; ++i )
	{
		if( CObjectManage::GetInstance()->Get_CharactorList()[i]->Get_Active() )
		{
			// ���� ��ġ - ���� ����
			pos = CObjectManage::GetInstance()->Get_CharactorList()[i]->Get_CharaPos() - a_pMonster->Get_Pos();

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

	// ���� �����̿� �ִ� ��ǥ�� ���� ������ ������ ���� ���·� ��ȯ
	if( min < 50.0f )
	{
		a_pMonster->Set_Target( Target );
		a_pMonster->Set_TargetDistance( min );
		a_pMonster->GetFSM()->ChangeState( Battle::GetInstance() );
	}

	// ������ ������ ���� ����� ��ǥ �߰�
	else if( min >= 50.0f && min <= 500.0f )
	{	
	
//			pMonster->Set_Target( Target );
//			D3DXVECTOR3 UnitVector = CObjectManage::GetInstance()->Get_CharactorList()[Target]->Get_CharaPos() - pMonster->Get_Pos();
//			D3DXVec3Normalize( &UnitVector, &UnitVector );
//			pMonster->Set_TargetUnitVector( UnitVector );
//			pMonster->Set_TargetPos( CObjectManage::GetInstance()->Get_CharactorList()[Target]->Get_CharaPos() );
//			pMonster->GetFSM()->ChangeState( Chase::GetInstance() );
	
			// ��ġ�� 0, 0 �������� ���� �� ����Ѵ�.
			INT StartX = INT( a_pMonster->Get_Pos().x + 510.0f ) / 10;
			INT StartZ = INT( a_pMonster->Get_Pos().z + 510.0f ) / 10;
			INT EndX = INT( CObjectManage::GetInstance()->Get_CharactorList()[Target]->Get_CharaPos().x + 510.0f ) / 10;
			INT EndZ = INT( CObjectManage::GetInstance()->Get_CharactorList()[Target]->Get_CharaPos().z + 510.0f ) / 10;

			a_pMonster->Set_TargetPos( EndX, EndZ );
	
			DWORD oldTime = timeGetTime();
			PathNode* path = Astar::GetInstance()->findPath( StartX, StartZ, EndX, EndZ );
			FLOAT time = ( timeGetTime() - oldTime ) * 0.001f;
			CDebugConsole::GetInstance()->Messagef( "Search Time : %f \n", time );

			// ���� Path ǥ�ø� ���ش�.
			ClearPath( a_pMonster->Get_Path() );
			Astar::GetInstance()->removePath( a_pMonster->Get_Path() );
			Astar::GetInstance()->clearMap();

			// �� Path�� ǥ���Ѵ�.
			SetPath( path );

			a_pMonster->Set_Path( path );

			// Path�� ������ Chase ���·�
			if( path )
			{
				a_pMonster->GetFSM()->ChangeState( Chase::GetInstance() );
				Chase::GetInstance()->Enter( a_pMonster );
			}
	}
	else
	{
//		a_pMonster->ChangeAnimation( 0 );
	}

	CDebugInterface::GetInstance()->AddMessageFloat( "length", min );
}


VOID Seek::Exit( CMonster* a_pMonster )
{

}


VOID Seek::Initialize( TileMap* a_pTileMap )
{
	m_pTileMap = a_pTileMap;
}


VOID Seek::ClearPath( PathNode* a_pPath )
{
	PathNode* temp = a_pPath;
	if( temp )
	{
		while( temp->next != NULL )
		{
			m_pTileMap->SetInfo( temp->x, temp->y, TileMap::TLM_WAY );

			temp = temp->next;
			if( temp == NULL )
				break;
		}
	}
}


VOID Seek::SetPath( PathNode* a_pPath )
{
	PathNode* temp = a_pPath;
	if( temp )
	{
		while( temp->next != NULL )
		{
			m_pTileMap->SetInfo( temp->x, temp->y, TileMap::TLM_COURSE );

			temp = temp->next;
			if( temp == NULL )
				break;
		}
	}
}