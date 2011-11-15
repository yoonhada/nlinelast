#ifndef _STIFFEN_H_
#define _STIFFEN_H_

#include "State.h"
#include "Monster.h"


class Stiffen : public State<CMonster>
{
private:
	Stiffen(){}

	Stiffen( const Stiffen& a_rhs );
	Stiffen& operator=( const Stiffen& a_rhs );


public:
	virtual ~Stiffen(){}

	static Stiffen* GetInstance();

	virtual VOID Enter( CMonster* a_pMonster );
	virtual VOID Execute( CMonster* a_pMonster );
	virtual VOID Exit( CMonster* a_pMonster );
};


#endif