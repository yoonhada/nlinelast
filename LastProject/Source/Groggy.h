#ifndef _GROGGY_H_
#define _GROGGY_H_

#include "State.h"
#include "Monster.h"


class Groggy : public State<CMonster>
{
private:
	Groggy(){}

	Groggy( const Groggy& a_rhs );
	Groggy& operator=( const Groggy& a_rhs );


public:
	virtual ~Groggy(){}

	static Groggy* GetInstance();

	virtual VOID Enter( CMonster* a_pMonster );
	virtual VOID Execute( CMonster* a_pMonster );
	virtual VOID Exit( CMonster* a_pMonster );
};


#endif