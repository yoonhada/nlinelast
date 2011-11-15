#ifndef _BATTLE_H_
#define _BATTLE_H_

#include "State.h"
#include "Monster.h"


class Battle : public State<CMonster>
{
private:
	Battle(){}

	Battle( const Battle& a_rhs );
	Battle& operator=( const Battle& a_rhs );


public:
	virtual ~Battle(){}

	static Battle* GetInstance();

	virtual VOID Enter( CMonster* a_pMonster );
	virtual VOID Execute( CMonster* a_pMonster );
	virtual VOID Exit( CMonster* a_pMonster );
};


#endif