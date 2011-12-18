#ifndef _MELEE3_H_
#define _MELEE3_H_

#include "State.h"
#include "Monster.h"


class Melee3 : public State<CMonster>
{
private:
	Melee3(){}

	Melee3( const Melee3& a_rhs );
	Melee3& operator=( const Melee3& a_rhs );


public:
	virtual ~Melee3(){}

	static Melee3* GetInstance();

	virtual VOID Enter( CMonster* a_pMonster );
	virtual VOID Execute( CMonster* a_pMonster );
	virtual VOID Exit( CMonster* a_pMonster );
};


#endif