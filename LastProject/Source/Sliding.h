#ifndef _SLIDING_H_
#define _SLIDING_H_

#include "State.h"


class Sliding : public State<CMonster>
{
private:
	Sliding(){}

	Sliding( const Sliding& a_rhs );
	Sliding& operator=( const Sliding& a_rhs );


public:
	virtual ~Sliding(){}

	static Sliding* GetInstance();

	virtual VOID Enter( CMonster* a_pMonster );
	virtual VOID Execute( CMonster* a_pMonster );
	virtual VOID Exit( CMonster* a_pMonster );
};


#endif