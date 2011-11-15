#ifndef _MELEE_H_
#define _MELEE_H_

#include "State.h"
#include "Monster.h"


class Melee : public State<CMonster>
{
private:
	Melee(){}

	Melee( const Melee& a_rhs );
	Melee& operator=( const Melee& a_rhs );


public:
	virtual ~Melee(){}

	static Melee* GetInstance();

	virtual VOID Enter( CMonster* a_pMonster );
	virtual VOID Execute( CMonster* a_pMonster );
	virtual VOID Exit( CMonster* a_pMonster );
};


#endif