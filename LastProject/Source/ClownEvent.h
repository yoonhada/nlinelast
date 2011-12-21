#ifndef _CLOWNEVENT_H_
#define _CLOWNEVENT_H_

#include "State.h"
#include "Monster.h"


class ClownEvent : public State<CMonster>
{
private:
	ClownEvent(){}

	ClownEvent( const ClownEvent& a_rhs );
	ClownEvent& operator=( const ClownEvent& a_rhs );


public:
	virtual ~ClownEvent(){}

	static ClownEvent* GetInstance();

	virtual VOID Enter( CMonster* a_pMonster );
	virtual VOID Execute( CMonster* a_pMonster );
	virtual VOID Exit( CMonster* a_pMonster );
};


#endif