#ifndef _MELEE4_H_
#define _MELEE4_H_

#include "State.h"
#include "Monster.h"


class Melee4 : public State<CMonster>
{
private:
	Melee4(){}

	Melee4( const Melee4& a_rhs );
	Melee4& operator=( const Melee4& a_rhs );


public:
	virtual ~Melee4(){}

	static Melee4* GetInstance();

	virtual VOID Enter( CMonster* a_pMonster );
	virtual VOID Execute( CMonster* a_pMonster );
	virtual VOID Exit( CMonster* a_pMonster );
};


#endif