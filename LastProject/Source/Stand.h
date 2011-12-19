#ifndef _STAND_H_
#define _STAND_H_

#include "State.h"
#include "Monster.h"


class Stand : public State<CMonster>
{
private:
	Stand(){}

	Stand( const Stand& a_rhs );
	Stand& operator=( const Stand& a_rhs );


public:
	virtual ~Stand(){}

	static Stand* GetInstance();

	virtual VOID Enter( CMonster* a_pMonster );
	virtual VOID Execute( CMonster* a_pMonster );
	virtual VOID Exit( CMonster* a_pMonster );
};


#endif