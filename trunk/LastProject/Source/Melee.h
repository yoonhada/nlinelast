#ifndef _MELEE_H_
#define _MELEE_H_

#include "State.h"
#include "Monster.h"


class Melee : public State<CMonster>
{
private:
	Melee(){}

	Melee( const Melee& rhs );
	Melee& operator=( const Melee& rhs );


public:
	virtual ~Melee(){}

	static Melee* GetInstance();

	virtual VOID Enter( CMonster* pMonster );
	virtual VOID Execute( CMonster* pMonster );
	virtual VOID Exit( CMonster* pMonster );
};


#endif