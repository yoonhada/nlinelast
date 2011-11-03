#ifndef _STATE_H_
#define _STATE_H_


template<class entity_type>
class State
{
public:
	State(){}
	virtual ~State(){}

	virtual VOID Enter( entity_type* pEntity )=0;
	virtual VOID Execute( entity_type* pEntity )=0;
	virtual VOID Exit( entity_type* pEntity )=0;
};


#endif