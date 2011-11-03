#ifndef _LOCKON_H_
#define _LOCKON_H_

#include "State.h"
#include "Monster.h"


class LockOn : public State<CMonster>
{
private:
	LockOn(){}

	LockOn( const LockOn& rhs );
	LockOn& operator=( const LockOn& rhs );


public:
	virtual ~LockOn(){}

	static LockOn* GetInstance();

	virtual VOID Enter( CMonster* pMonster );
	virtual VOID Execute( CMonster* pMonster );
	virtual VOID Exit( CMonster* pMonster );
};


#endif