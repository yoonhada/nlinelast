#ifndef _DASHREADY_H_
#define _DASHREADY_H_

#include "State.h"


class DashReady : public State<CMonster>
{
private:
	DashReady(){}

	DashReady( const DashReady& a_rhs );
	DashReady& operator=( const DashReady& a_rhs );


public:
	virtual ~DashReady(){}

	static DashReady* GetInstance();

	virtual VOID Enter( CMonster* a_pMonster );
	virtual VOID Execute( CMonster* a_pMonster );
	virtual VOID Exit( CMonster* a_pMonster );

	FLOAT SetAngle( CMonster* a_pMonster );
};


#endif