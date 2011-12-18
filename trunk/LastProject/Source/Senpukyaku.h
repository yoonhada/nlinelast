#ifndef _SENPUKYAKU_H_
#define _SENPUKYAKU_H_

#include "State.h"
#include "Monster.h"


class Senpukyaku : public State<CMonster>
{
private:
	Senpukyaku(){}

	Senpukyaku( const Senpukyaku& a_rhs );
	Senpukyaku& operator=( const Senpukyaku& a_rhs );


public:
	virtual ~Senpukyaku(){}

	static Senpukyaku* GetInstance();

	virtual VOID Enter( CMonster* a_pMonster );
	virtual VOID Execute( CMonster* a_pMonster );
	virtual VOID Exit( CMonster* a_pMonster );
};


#endif