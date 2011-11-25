#ifndef _CHASE_H_
#define _CHASE_H_

#include "State.h"
#include "Monster.h"


class Chase : public State<CMonster>
{
private:
	Chase(){}

	Chase( const Chase& a_rhs );
	Chase& operator=( const Chase& a_rhs );

	PathNode*	m_pCurrentPath;
	D3DXVECTOR3 m_vPos;
	INT			m_iCurrentX;
	INT			m_iCurrentZ;

	ASEViewerBase::LPGRAPHINFO m_pMapInfo;


public:
	virtual ~Chase(){}

	static Chase* GetInstance();

	virtual VOID Enter( CMonster* a_pMonster );
	virtual VOID Execute( CMonster* a_pMonster );
	virtual VOID Exit( CMonster* a_pMonster );

	VOID Initialize( ASEViewerBase::LPGRAPHINFO a_pMapInfo );
	FLOAT GetDegree();
	BOOL isArrive( CMonster* a_pMonster );
};


#endif