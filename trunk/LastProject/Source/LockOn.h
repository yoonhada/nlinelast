#ifndef _LOCKON_H_
#define _LOCKON_H_

#include "State.h"
#include "Monster.h"


class LockOn : public State<CMonster>
{
private:
	LockOn(){}

	LockOn( const LockOn& a_rhs );
	LockOn& operator=( const LockOn& a_rhs );


public:
	virtual ~LockOn(){}

	static LockOn* GetInstance();

	virtual VOID Enter( CMonster* a_pMonster );
	virtual VOID Execute( CMonster* a_pMonster );
	virtual VOID Exit( CMonster* a_pMonster );

	BOOL isInSight();
};


#endif