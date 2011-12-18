#ifndef _COMBO3_H_
#define _COMBO3_H_

#include "State.h"
#include "Monster.h"


class Combo3 : public State<CMonster>
{
private:
	Combo3(){}

	Combo3( const Combo3& a_rhs );
	Combo3& operator=( const Combo3& a_rhs );


public:
	virtual ~Combo3(){}

	static Combo3* GetInstance();

	virtual VOID Enter( CMonster* a_pMonster );
	virtual VOID Execute( CMonster* a_pMonster );
	virtual VOID Exit( CMonster* a_pMonster );
};


#endif