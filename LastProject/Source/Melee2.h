#ifndef _MELEE2_H_
#define _MELEE2_H_

#include "State.h"
#include "Monster.h"


class Melee2 : public State<CMonster>
{
private:
	Melee2(){}

	Melee2( const Melee2& a_rhs );
	Melee2& operator=( const Melee2& a_rhs );


public:
	virtual ~Melee2(){}

	static Melee2* GetInstance();

	virtual VOID Enter( CMonster* a_pMonster );
	virtual VOID Execute( CMonster* a_pMonster );
	virtual VOID Exit( CMonster* a_pMonster );
};


#endif