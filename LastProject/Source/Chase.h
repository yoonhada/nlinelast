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

	PathNode*	m_pNextPath;
	D3DXVECTOR3 m_vCurrentPos;
	D3DXVECTOR3 m_vPreviousPos;
	INT			m_iCurrentX;
	INT			m_iCurrentZ;
	INT			m_iTotalPathCnt;
	FLOAT		m_fCurrentAngle;
	FLOAT		m_fNextAngle;
	FLOAT		m_fAngle0;
	FLOAT		m_fAngle1;

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