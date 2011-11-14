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


VOID Seek::Enter( CMonster* pMonster )
{

}


VOID Seek::Execute( CMonster* pMonster )
{
	INT Target = -1;
	D3DXVECTOR3 pos;
	FLOAT length = 0.0f;
	static FLOAT min = 0.0f;

	// 몬스터와 가장 가까이에 있는 유저를 찾는다.
	for( INT i=0; i<4; ++i )
	{
		if( CObjectManage::GetInstance()->Get_CharactorList()[i]->Get_Active() )
		{
			// 유저 위치 - 보스 몬스터
			pos = CObjectManage::GetInstance()->Get_CharactorList()[i]->Get_CharaPos() - pMonster->Get_Pos();

			// 거리 구하기
			length = D3DXVec3Length( &pos );

			if( i == 0 )
			{
				min = length;
				Target = i;
			}
			else
			{
				// 더 가까이 있으면
				if( length < min )
				{
					min = length;
					Target = i;
				}
			}
		}
	}

	// 가장 가까이에 있는 유저가 공격 범위에 있으면 전투 상태로 전환
	if( min < 50.0f )
	{
		pMonster->Set_Target( Target );
		pMonster->Set_TargetDistance( min );
		pMonster->GetFSM()->ChangeState( Battle::GetInstance() );
	}
	// 범위에 없으면 가장 가까운 유저 추격
	else if( min >= 50.0f && min <= 500.0f )
	{	
	/*
			pMonster->Set_Target( Target );
			D3DXVECTOR3 UnitVector = CObjectManage::GetInstance()->Get_CharactorList()[Target]->Get_CharaPos() - pMonster->Get_Pos();
			D3DXVec3Normalize( &UnitVector, &UnitVector );
			pMonster->Set_TargetUnitVector( UnitVector );
			pMonster->Set_TargetPos( CObjectManage::GetInstance()->Get_CharactorList()[Target]->Get_CharaPos() );
			pMonster->GetFSM()->ChangeState( Chase::GetInstance() );
	*/
			// 위치를 0, 0 기준으로 맞춘 후 계산한다.
			INT StartX = INT( pMonster->Get_Pos().x + 510.0f ) / 10;
			INT StartZ = INT( pMonster->Get_Pos().z + 510.0f ) / 10;
			INT EndX = INT( CObjectManage::GetInstance()->Get_CharactorList()[Target]->Get_CharaPos().x + 510.0f ) / 10;
			INT EndZ = INT( CObjectManage::GetInstance()->Get_CharactorList()[Target]->Get_CharaPos().z + 510.0f ) / 10;

			pMonster->Set_TargetPos( EndX, EndZ );
	
			DWORD oldTime = timeGetTime();
			PathNode* path = Astar::GetInstance()->findPath( StartX, StartZ, EndX, EndZ );
			FLOAT time = ( timeGetTime() - oldTime ) * 0.001f;
			CDebugConsole::GetInstance()->Messagef( "Search Time : %f \n", time );

			// 이전 Path 표시를 없앤다.
			ClearPath( pMonster->Get_Path() );
			Astar::GetInstance()->removePath( pMonster->Get_Path() );
			Astar::GetInstance()->clearMap();

			// 새 Path를 표시한다.
			SetPath( path );

			pMonster->Set_Path( path );

			// Path가 있으면 Chase 상태로
			if( path )
			{
				pMonster->GetFSM()->ChangeState( Chase::GetInstance() );
				Chase::GetInstance()->Enter( pMonster );
			}
	}
	else
	{
//		pMonster->ChangeAnimation( 0 );
	}

	CDebugInterface::GetInstance()->AddMessageFloat( "length", min );
}


VOID Seek::Exit( CMonster* pMonster )
{

}


VOID Seek::Initialize( TileMap* pTileMap )
{
	m_pTileMap = pTileMap;
}


VOID Seek::ClearPath( PathNode* pPath )
{
	PathNode* temp = pPath;
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


VOID Seek::SetPath( PathNode* pPath )
{
	PathNode* temp = pPath;
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