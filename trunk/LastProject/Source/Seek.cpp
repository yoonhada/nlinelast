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
	// ��� �ִϸ��̼����� �ٲ۴�.
	//a_pMonster->ChangeAnimation( CMonster::ANIM_STAND );
}


VOID Seek::Execute( CMonster* a_pMonster )
{
	INT Target = -1;
	D3DXVECTOR3 pos( 0.0f, 0.0f, 0.0f );
	FLOAT length = 0.0f;
	FLOAT min = 99999999999.0f;

	// ���Ϳ� ���� �����̿� �ִ� ������ ã�´�.
	INT nClientNum;
	CCharactor * pCharactors = CObjectManage::GetInstance()->Get_Charactor();	
	for( INT i=0; i<4; ++i )
	{		
		nClientNum = CObjectManage::GetInstance()->Get_CharTable( i );
		if( nClientNum != -1 )
		{
			// ���Ϳ� ������ �Ÿ�.�� ���Ѵ�.
			pos = pCharactors[ nClientNum ].Get_CharaPos() - a_pMonster->Get_Pos();
			length = D3DXVec3Length( &pos );

			if( length < min )
			{
				// �� ������ ������
				min = length;
				Target = nClientNum;
			}
		}
	}

	// ���õ� Ÿ���� ������ ����
	if( Target == -1 )
	{
		return;
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
		D3DXVECTOR3 vMonsterPos = a_pMonster->Get_Pos();
		D3DXVECTOR3 vPlayerPos = pCharactors[Target].Get_CharaPos();
		D3DXVECTOR3 vEnd = m_pTileMap->GetInfo()->vecEnd;
		FLOAT fTileSize = m_pTileMap->GetInfo()->fTileSize;

		// ��ġ�� 0, 0 �������� ���� �� ����Ѵ�.
		INT StartX	= INT( ( vMonsterPos.x + vEnd.x ) / fTileSize );
		INT StartZ	= INT( ( vEnd.z - vMonsterPos.z ) / fTileSize );
		INT EndX	= INT( ( vPlayerPos.x + vEnd.x )  / fTileSize );
		INT EndZ	= INT( ( vEnd.z - vPlayerPos.z )  / fTileSize );

#ifdef _DEBUG
		static FLOAT sx = 0;
		static FLOAT sz = 0;
		static FLOAT ex = 0;
		static FLOAT ez = 0;
		
		sx = (FLOAT)StartX;
		sz = (FLOAT)StartZ;
		ex = (FLOAT)EndX;
		ez = (FLOAT)EndZ;

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