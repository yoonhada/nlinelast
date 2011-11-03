#ifndef _SPIN_H_
#define _SPIN_H_

#include "State.h"
#include "Monster.h"


class Spin : public State<CMonster>
{
private:
	Spin(){}

	Spin( const Spin& rhs );
	Spin& operator=( const Spin& rhs );


public:
	virtual ~Spin(){}

	static Spin* GetInstance();

	virtual VOID Enter( CMonster* pMonster );
	virtual VOID Execute( CMonster* pMonster );
	virtual VOID Exit( CMonster* pMonster );
};


#endif