#ifndef _SPINGROGGY_H_
#define _SPINGROGGY_H_

#include "State.h"
#include "Monster.h"


class SpinGroggy : public State<CMonster>
{
private:
	SpinGroggy(){}

	SpinGroggy( const SpinGroggy& a_rhs );
	SpinGroggy& operator=( const SpinGroggy& a_rhs );


public:
	virtual ~SpinGroggy(){}

	static SpinGroggy* GetInstance();

	virtual VOID Enter( CMonster* a_pMonster );
	virtual VOID Execute( CMonster* a_pMonster );
	virtual VOID Exit( CMonster* a_pMonster );
};


#endif