#ifndef _COMBO2_H_
#define _COMBO2_H_

#include "State.h"
#include "Monster.h"


class Combo2 : public State<CMonster>
{
private:
	Combo2(){}

	Combo2( const Combo2& a_rhs );
	Combo2& operator=( const Combo2& a_rhs );


public:
	virtual ~Combo2(){}

	static Combo2* GetInstance();

	virtual VOID Enter( CMonster* a_pMonster );
	virtual VOID Execute( CMonster* a_pMonster );
	virtual VOID Exit( CMonster* a_pMonster );
};


#endif