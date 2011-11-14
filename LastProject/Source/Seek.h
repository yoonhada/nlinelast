#ifndef _SEEK_H_
#define _SEEK_H_

#include "State.h"
#include "Monster.h"

#include "TileMap.h"


class Seek : public State<CMonster>
{
private:
	Seek(){}

	Seek( const Seek& rhs );
	Seek& operator=( const Seek& rhs );

	TileMap* m_pTileMap;


public:
	virtual ~Seek(){}

	static Seek* GetInstance();

	virtual VOID Enter( CMonster* pMonster );
	virtual VOID Execute( CMonster* pMonster );
	virtual VOID Exit( CMonster* pMonster );

	// AI 테스트용
	VOID Initialize( TileMap* pTileMap );
	VOID ClearPath( PathNode* pPath );
	VOID SetPath( PathNode* pPath );
};


#endif