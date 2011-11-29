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
	CCharactor *pCharactors = CObjectManage::GetInstance()->Get_Charactors();	
	for( INT i=0; i<4; ++i )
	{
		if( pCharactors[i].Get_Active() )
		{
			// ���� ��ġ - ���� ����
			pos = pCharactors[i].Get_CharaPos() - a_pMonster->Get_Pos();

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
	if( min < 25.0f )
	{
		a_pMonster->Set_Target( Target );
		a_pMonster->Set_TargetDistance( min );
		a_pMonster->GetFSM()->ChangeState( Battle::GetInstance() );
	}

	// ������ ������ ���� ����� ��ǥ �߰�
	else if( min >= 25.0f && min <= 500.0f )
	{
		// ��ġ�� 0, 0 �������� ���� �� ����Ѵ�.
		INT StartX = INT( a_pMonster->Get_Pos().x + m_pTileMap->GetInfo()->vecEnd.x ) / m_pTileMap->GetInfo()->fTileSize;
		INT StartZ = INT( -( a_pMonster->Get_Pos().z - m_pTileMap->GetInfo()->vecEnd.z ) ) / m_pTileMap->GetInfo()->fTileSize;
		INT EndX = INT( pCharactors[Target].Get_CharaPos().x + m_pTileMap->GetInfo()->vecEnd.x ) / m_pTileMap->GetInfo()->fTileSize;
		INT EndZ = INT( -( pCharactors[Target].Get_CharaPos().z - m_pTileMap->GetInfo()->vecEnd.z ) ) / m_pTileMap->GetInfo()->fTileSize;

#ifdef _DEBUG
		static FLOAT sx = 0;
		static FLOAT sz = 0;
		static FLOAT ex = 0;
		static FLOAT ez = 0;
		
		sx = StartX;
		sz = StartZ;
		ex = EndX;
		ez = EndZ;

		CDebugInterface::GetInstance()->AddMessageFloat( "sx", sx );
		CDebugInterface::GetInstance()->AddMessageFloat( "sz", sz );
		CDebugInterface::GetInstance()->AddMessageFloat( "ex", ex );
		CDebugInterface::GetInstance()->AddMessageFloat( "ez", ez );
#endif

		a_pMonster->Set_TargetPos( EndX, EndZ );

		DWORD oldTime = timeGetTime();
		PathNode* path = Astar::GetInstance()->findPath( StartX, StartZ, EndX, EndZ );
		FLOAT time = ( timeGetTime() - oldTime ) * 0.001f;
		CDebugConsole::GetInstance()->Messagef( "Search Time : %f \n", time );

#ifdef _DEBUG
		// ���� Path ǥ�ø� ���ش�.
		ClearPath( a_pMonster->Get_Path() );
		Astar::GetInstance()->removePath( a_pMonster->Get_Path() );
		Astar::GetInstance()->clearMap();

		// �� Path�� ǥ���Ѵ�.
		SetPath( path );
#endif

		a_pMonster->Set_Path( path );

		// Path�� ������ Chase ���·�
		if( path )
		{
			a_pMonster->GetFSM()->ChangeState( Chase::GetInstance() );
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
		while( temp != NULL )
		{
			m_pTileMap->SetInfo( TileMap::TLM_WAY, temp->x, temp->y );

			temp = temp->next;
		}
	}
}


VOID Seek::SetPath( PathNode* a_pPath )
{
	PathNode* temp = a_pPath;
	if( temp )
	{
		while( temp != NULL )
		{
			m_pTileMap->SetInfo( TileMap::TLM_COURSE, temp->x, temp->y );

			temp = temp->next;
		}
	}
}