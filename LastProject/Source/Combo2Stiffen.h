#ifndef _COMBO2STIFFEN_H_
#define _COMBO2STIFFEN_H_

#include "State.h"
#include "Monster.h"


class Combo2Stiffen : public State<CMonster>
{
private:
	Combo2Stiffen(){}

	Combo2Stiffen( const Combo2Stiffen& a_rhs );
	Combo2Stiffen& operator=( const Combo2Stiffen& a_rhs );


public:
	virtual ~Combo2Stiffen(){}

	static Combo2Stiffen* GetInstance();

	virtual VOID Enter( CMonster* a_pMonster );
	virtual VOID Execute( CMonster* a_pMonster );
	virtual VOID Exit( CMonster* a_pMonster );
};


#endif