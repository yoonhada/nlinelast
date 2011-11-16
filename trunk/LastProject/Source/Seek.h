#ifndef _SEEK_H_
#define _SEEK_H_

#include "State.h"
#include "Monster.h"

#include "TileMap.h"


class Seek : public State<CMonster>
{
private:
	Seek(){}

	Seek( const Seek& a_rhs );
	Seek& operator=( const Seek& a_rhs );

	TileMap* m_pTileMap;


public:
	virtual ~Seek(){}

	static Seek* GetInstance();

	virtual VOID Enter( CMonster* a_pMonster );
	virtual VOID Execute( CMonster* a_pMonster );
	virtual VOID Exit( CMonster* a_pMonster );

	// AI �׽�Ʈ��
	VOID Initialize( TileMap* a_pTileMap );
	VOID ClearPath( PathNode* a_pPath );
	VOID SetPath( PathNode* a_pPath );
};


#endif