#ifndef _ROCKETPUNCH_H_
#define _ROCKETPUNCH_H_

#include "State.h"
#include "Monster.h"


class RocketPunch : public State<CMonster>
{
private:
	RocketPunch(){}

	RocketPunch( const RocketPunch& a_rhs );
	RocketPunch& operator=( const RocketPunch& a_rhs );


public:
	virtual ~RocketPunch(){}

	static RocketPunch* GetInstance();

	virtual VOID Enter( CMonster* a_pMonster );
	virtual VOID Execute( CMonster* a_pMonster );
	virtual VOID Exit( CMonster* a_pMonster );

	FLOAT SetAngle( CMonster* a_pMonster );
};


#endif