#ifndef _SEEK_H_
#define _SEEK_H_

#include "State.h"
#include "Monster.h"

#include "ASEViewerBase.h"
#include "TileMap.h"


class Seek : public State<CMonster>
{
private:
	Seek(){}

	Seek( const Seek& a_rhs );
	Seek& operator=( const Seek& a_rhs );

	ASEViewerBase::LPGRAPHINFO m_pMapInfo;
	TileMap*	m_pTileMap;


public:
	virtual ~Seek(){}

	static Seek* GetInstance();

	virtual VOID Enter( CMonster* a_pMonster );
	virtual VOID Execute( CMonster* a_pMonster );
	virtual VOID Exit( CMonster* a_pMonster );

	// AI 테스트용
	VOID Initialize( ASEViewerBase::LPGRAPHINFO a_pMapInfo, TileMap* a_pTileMap );
	VOID ClearPath( PathNode* a_pPath );
	VOID SetPath( PathNode* a_pPath );
};


#endif