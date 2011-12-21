#include "stdafx.h"

#include "ClownEvent.h"

#include "Monster.h"


ClownEvent* ClownEvent::GetInstance()
{
	static ClownEvent Instance;

	return &Instance;
}


VOID ClownEvent::Enter( CMonster* a_pMonster )
{
	a_pMonster->ChangeAnimation( CMonster::ANIM_EVENT_CLOWN );
#ifdef _DEBUG
	////CDebugConsole::GetInstance()->Messagef( L"ClownEvent : ANIM_EVENT_CLOWN \n" );
#endif
}


VOID ClownEvent::Execute( CMonster* a_pMonster )
{
	a_pMonster->Set_UpdateTime();

	FLOAT t = a_pMonster->Get_Time();
	if( t >= 4.0f )
	{
		a_pMonster->Set_ClearTime();

		CSound::GetInstance()->PlayEffect( CSound::EFFECT_CLOWN_LAUGH2 );
	}
}


VOID ClownEvent::Exit( CMonster* a_pMonster )
{

}