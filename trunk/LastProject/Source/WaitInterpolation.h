#ifndef _WAITINTERPOLATION_H_
#define _WAITINTERPOLATION_H_

#include "State.h"
#include "Monster.h"


class WaitInterPolation : public State<CMonster>
{
private:
	WaitInterPolation(){}

	WaitInterPolation( const WaitInterPolation& rhs );
	WaitInterPolation& operator=( const WaitInterPolation& rhs );


public:
	virtual ~WaitInterPolation(){}

	static WaitInterPolation* GetInstance();

	virtual VOID Enter( CMonster* pMonster );
	virtual VOID Execute( CMonster* pMonster );
	virtual VOID Exit( CMonster* pMonster );
};


#endif