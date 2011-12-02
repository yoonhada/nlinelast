#ifndef _STATEMACHINE_H_
#define _STATEMACHINE_H_

#include "State.h"


template<class entity_type>
class StateMachine
{
private:
	entity_type*		m_pOwner;			// 이 인스턴스를 소유하는 에이전트
	State<entity_type>*	m_pPreviousState;	// 이전 상태
	State<entity_type>*	m_pCurrentState;	// 현재 상태
	State<entity_type>*	m_pGlobalState;		// 이 상태는 FSM이 갱신될 때마다 호출된다.


public:
	StateMachine( entity_type* pOwner ) : m_pOwner( pOwner ),
										  m_pPreviousState( NULL ),
										  m_pCurrentState( NULL ),
										  m_pGlobalState( NULL )
	{}

	virtual ~StateMachine(){}


	VOID Update() const
	{
		if( m_pGlobalState )	m_pGlobalState->Execute( m_pOwner );
		if( m_pCurrentState )	m_pCurrentState->Execute( m_pOwner );
	}


	VOID ChangeState( State<entity_type>* pNewState )
	{
		// 현재 상태를 저장해 둔다.
		m_pPreviousState = m_pCurrentState;

		// 현재 상태 Exit
		m_pCurrentState->Exit( m_pOwner );

		// 새로운 상태를 현재 상태로
		m_pCurrentState = pNewState;

		// 새로운 상태로 Enter
		if ( pNewState )
		{
			m_pCurrentState->Enter( m_pOwner );
		}
	}


	VOID RevertToPreviousState()
	{
		ChangeState( m_pPreviousState );
	}


	BOOL isInState( const State<entity_type>& state ) const
	{
		if( typeid( &m_pCurrentState ) == typeid( state ) )
		{
			return TRUE;
		}

		return FALSE;
	}


	std::string GetNameOfCurrentState() const
	{
		std::string s( typeid(*m_pCurrentState).name() );

		// 문자열 앞의 'class' 를 제거한다.
		if( s.size() > 5 )
		{
			s.erase( 0, 6 );
		}

		return s;
	}


	// Set
	VOID SetPreviousState( State<entity_type>* state )	{ m_pPreviousState = state; }
	VOID SetCurrentState( State<entity_type>* state )	{ m_pCurrentState = state; }
	VOID SetGlobalState( State<entity_type>* state )	{ m_pGlobalState = state; }


	// Get
	State<entity_type>* GetPreviousState() const	{ return m_pPreviousState; }
	State<entity_type>* GetCurrentState() const		{ return m_pCurrentState; }
	State<entity_type>* GetGlobalState() const		{ return m_pGlobalState; }
};


#endif
