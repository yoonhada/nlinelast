#include "stdafx.h"

#include "SpinGroggy.h"
#include "Seek.h"

#include "Monster.h"


SpinGroggy* SpinGroggy::GetInstance()
{
	static SpinGroggy Instance;

	return &Instance;
}


VOID SpinGroggy::Enter( CMonster* a_pMonster )
{
	// Spin ���� �ִϸ��̼����� �ٲ۴�.
	a_pMonster->ChangeAnimation( CMonster::ANIM_SPIN_GROGGY );
#ifdef _DEBUG
	CDebugConsole::GetInstance()->Messagef( L"Groggy : ANIM_SPIN_GROGGY \n" );
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


VOID SpinGroggy::Execute( CMonster* a_pMonster )
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


VOID SpinGroggy::Exit( CMonster* a_pMonster )
{

}