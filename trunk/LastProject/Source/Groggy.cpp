#include "stdafx.h"

#include "Groggy.h"
#include "Seek.h"

#include "Monster.h"


Groggy* Groggy::GetInstance()
{
	static Groggy Instance;

	return &Instance;
}


VOID Groggy::Enter( CMonster* a_pMonster )
{
	// Spin ���� �ִϸ��̼����� �ٲ۴�.
	a_pMonster->ChangeAnimation( CMonster::ANIM_GROGGY );
#ifdef _DEBUG
	CDebugConsole::GetInstance()->Messagef( L"Groggy : ANIM_GROGGY \n" );
#endif

	if( a_pMonster->Get_MonsterNumber() == 2 )
	{
		CSound::GetInstance()->PlayEffect( 0 );
	}
	else
	{
		CSound::GetInstance()->PlayEffect( 0 );
	}
}


VOID Groggy::Execute( CMonster* a_pMonster )
{
	a_pMonster->Set_UpdateTime();

	// �׷α� ���°� �������� �ٽ� Ž�� ���·�
	FLOAT t = a_pMonster->Get_Time();
	if( t >= 4.0f )
	{
		a_pMonster->Set_ClearTime();

		// ȣ��Ʈ�̸� Ž�� ���·�
		if( CObjectManage::GetInstance()->IsHost() == TRUE )
		{
			a_pMonster->GetFSM()->ChangeState( Seek::GetInstance() );
		}
		else
		{
			a_pMonster->GetFSM()->ChangeState( NULL );
		}
	}
}


VOID Groggy::Exit( CMonster* a_pMonster )
{

}