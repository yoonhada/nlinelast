#ifndef _COMBO1STIFFEN_H_
#define _COMBO1STIFFEN_H_

#include "State.h"
#include "Monster.h"


class Combo1Stiffen : public State<CMonster>
{
private:
	Combo1Stiffen(){}

	Combo1Stiffen( const Combo1Stiffen& a_rhs );
	Combo1Stiffen& operator=( const Combo1Stiffen& a_rhs );


public:
	virtual ~Combo1Stiffen(){}

	static Combo1Stiffen* GetInstance();

	virtual VOID Enter( CMonster* a_pMonster );
	virtual VOID Execute( CMonster* a_pMonster );
	virtual VOID Exit( CMonster* a_pMonster );
};


#endif