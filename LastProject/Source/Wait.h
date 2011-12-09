#ifndef _WAIT_H_
#define _WAIT_H_

#include "State.h"
#include "Monster.h"


class Wait : public State<CMonster>
{
private:
	Wait(){}

	Wait( const Wait& a_rhs );
	Wait& operator=( const Wait& a_rhs );


public:
	virtual ~Wait(){}

	static Wait* GetInstance();

	virtual VOID Enter( CMonster* a_pMonster );
	virtual VOID Execute( CMonster* a_pMonster );
	virtual VOID Exit( CMonster* a_pMonster );
};


#endif