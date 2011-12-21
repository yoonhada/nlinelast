#include "stdafx.h"

#include "SpinGroggy.h"
#include "Stand.h"

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
	//CDebugConsole::GetInstance()->Messagef( L"Groggy : ANIM_SPIN_GROGGY \n" );
#endif

	CSound::GetInstance()->PlayEffect( CSound::EFFECT_CLOWN_DOWN );
}


VOID SpinGroggy::Execute( CMonster* a_pMonster )
{
	static FLOAT fTime = 0.0f;
	fTime += CFrequency::GetInstance()->getFrametime();
	if( fTime >= 1.2f )
	{
		fTime = 0.0f;

		CSound::GetInstance()->PlayEffect( CSound::EFFECT_CLOWN_DOWN );
	}

	a_pMonster->Set_UpdateTime();

	// �׷α� ���°� �������� �ٽ� Ž�� ���·�
	FLOAT t = a_pMonster->Get_Time();
	if( t >= 2.0f )
	{
		a_pMonster->Set_ClearTime();

		// ȣ��Ʈ�̸� ��� ���·�
		if( CObjectManage::GetInstance()->IsHost() == TRUE )
		{
			a_pMonster->GetFSM()->ChangeState( Stand::GetInstance() );
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