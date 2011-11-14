#ifndef _CHASE_H_
#define _CHASE_H_

#include "State.h"
#include "Monster.h"


class Chase : public State<CMonster>
{
private:
	Chase(){}

	Chase( const Chase& rhs );
	Chase& operator=( const Chase& rhs );

	PathNode*	m_pCurrentPath;
	D3DXVECTOR3 m_vPos;
	INT			m_iCurrentX;
	INT			m_iCurrentZ;


public:
	virtual ~Chase(){}

	static Chase* GetInstance();

	virtual VOID Enter( CMonster* pMonster );
	virtual VOID Execute( CMonster* pMonster );
	virtual VOID Exit( CMonster* pMonster );

	FLOAT GetDegree();
	BOOL isArrive( CMonster* pMonster );
};


#endif