#ifndef _STIFFEN_H_
#define _STIFFEN_H_

#include "State.h"
#include "Monster.h"


class Stiffen : public State<CMonster>
{
private:
	Stiffen(){}

	Stiffen( const Stiffen& rhs );
	Stiffen& operator=( const Stiffen& rhs );


public:
	virtual ~Stiffen(){}

	static Stiffen* GetInstance();

	virtual VOID Enter( CMonster* pMonster );
	virtual VOID Execute( CMonster* pMonster );
	virtual VOID Exit( CMonster* pMonster );
};


#endif