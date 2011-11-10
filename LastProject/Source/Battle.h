#ifndef _BATTLE_H_
#define _BATTLE_H_

#include "State.h"
#include "Monster.h"


class Battle : public State<CMonster>
{
private:
	Battle(){}

	Battle( const Battle& rhs );
	Battle& operator=( const Battle& rhs );


public:
	virtual ~Battle(){}

	static Battle* GetInstance();

	virtual VOID Enter( CMonster* pMonster );
	virtual VOID Execute( CMonster* pMonster );
	virtual VOID Exit( CMonster* pMonster );
};


#endif