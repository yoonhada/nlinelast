#ifndef _SPIN_H_
#define _SPIN_H_

#include "State.h"
#include "Monster.h"


class Spin : public State<CMonster>
{
private:
	Spin(){}

	Spin( const Spin& a_rhs );
	Spin& operator=( const Spin& a_rhs );


public:
	virtual ~Spin(){}

	static Spin* GetInstance();

	virtual VOID Enter( CMonster* a_pMonster );
	virtual VOID Execute( CMonster* a_pMonster );
	virtual VOID Exit( CMonster* a_pMonster );
};


#endif