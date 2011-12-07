#ifndef _CHASE_H_
#define _CHASE_H_

#include "State.h"
#include "Monster.h"
#include "TileMap.h"


class Chase : public State<CMonster>
{
private:
	Chase(){}

	Chase( const Chase& a_rhs );
	Chase& operator=( const Chase& a_rhs );

	TileMap::INFO*	m_pMapInfo;


public:
	virtual ~Chase(){}

	static Chase* GetInstance();

	virtual VOID Enter( CMonster* a_pMonster );
	virtual VOID Execute( CMonster* a_pMonster );
	virtual VOID Exit( CMonster* a_pMonster );

	VOID		Initialize( TileMap::INFO* a_pMapInfo );
//	FLOAT		GetDegree();
//	D3DXVECTOR3 GetWorldPos( INT a_iX, INT a_iZ );
};


#endif