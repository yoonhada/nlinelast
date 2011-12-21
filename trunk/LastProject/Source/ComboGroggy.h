#ifndef _COMBOGROGGY_H_
#define _COMBOGROGGY_H_

#include "State.h"
#include "Monster.h"


class ComboGroggy : public State<CMonster>
{
private:
	ComboGroggy(){}

	ComboGroggy( const ComboGroggy& a_rhs );
	ComboGroggy& operator=( const ComboGroggy& a_rhs );


public:
	virtual ~ComboGroggy(){}

	static ComboGroggy* GetInstance();

	virtual VOID Enter( CMonster* a_pMonster );
	virtual VOID Execute( CMonster* a_pMonster );
	virtual VOID Exit( CMonster* a_pMonster );
};


#endif