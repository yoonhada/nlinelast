#ifndef _SENPUKYAKUREADY_H_
#define _SENPUKYAKUREADY_H_

#include "State.h"
#include "Monster.h"


class SenpukyakuReady : public State<CMonster>
{
private:
	SenpukyakuReady(){}

	SenpukyakuReady( const SenpukyakuReady& a_rhs );
	SenpukyakuReady& operator=( const SenpukyakuReady& a_rhs );


public:
	virtual ~SenpukyakuReady(){}

	static SenpukyakuReady* GetInstance();

	virtual VOID Enter( CMonster* a_pMonster );
	virtual VOID Execute( CMonster* a_pMonster );
	virtual VOID Exit( CMonster* a_pMonster );
};


#endif