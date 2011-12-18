#ifndef _COMBO1_H_
#define _COMBO1_H_

#include "State.h"
#include "Monster.h"


class Combo1 : public State<CMonster>
{
private:
	Combo1(){}

	Combo1( const Combo1& a_rhs );
	Combo1& operator=( const Combo1& a_rhs );


public:
	virtual ~Combo1(){}

	static Combo1* GetInstance();

	virtual VOID Enter( CMonster* a_pMonster );
	virtual VOID Execute( CMonster* a_pMonster );
	virtual VOID Exit( CMonster* a_pMonster );
};


#endif