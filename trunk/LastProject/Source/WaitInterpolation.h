#ifndef _WAITINTERPOLATION_H_
#define _WAITINTERPOLATION_H_

#include "State.h"
#include "Monster.h"


class WaitInterPolation : public State<CMonster>
{
private:
	WaitInterPolation(){}

	WaitInterPolation( const WaitInterPolation& a_rhs );
	WaitInterPolation& operator=( const WaitInterPolation& a_rhs );


public:
	virtual ~WaitInterPolation(){}

	static WaitInterPolation* GetInstance();

	virtual VOID Enter( CMonster* a_pMonster );
	virtual VOID Execute( CMonster* a_pMonster );
	virtual VOID Exit( CMonster* a_pMonster );
};


#endif