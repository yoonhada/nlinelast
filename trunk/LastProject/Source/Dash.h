#ifndef _DASH_H_
#define _DASH_H_

#include "State.h"


class Dash : public State<CMonster>
{
private:
	Dash(){}

	Dash( const Dash& a_rhs );
	Dash& operator=( const Dash& a_rhs );

	D3DXVECTOR3 m_vPos;
	D3DXVECTOR3 m_vNextPos;
	FLOAT		m_fT;


public:
	virtual ~Dash(){}

	static Dash* GetInstance();

	virtual VOID Enter( CMonster* a_pMonster );
	virtual VOID Execute( CMonster* a_pMonster );
	virtual VOID Exit( CMonster* a_pMonster );

	FLOAT SetAngle( CMonster* a_pMonster );
};


#endif