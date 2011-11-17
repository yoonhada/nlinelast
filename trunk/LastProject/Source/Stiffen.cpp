#include "stdafx.h"

#include "Stiffen.h"
#include "WaitInterPolation.h"

#include "Monster.h"
#include "Frequency.h"


Stiffen* Stiffen::GetInstance()
{
	static Stiffen Instance;

	return &Instance;
}


VOID Stiffen::Enter( CMonster* a_pMonster )
{

}


VOID Stiffen::Execute( CMonster* a_pMonster )
{
	BOOL bCheck = FALSE;

	if( a_pMonster->Get_AnimationEndCheck() == FALSE )
	{
		// �ִϸ��̼� 0�� ����
		a_pMonster->ChangeAnimation( 0 );

		bCheck = TRUE;

		// �������°� ������ �ִϸ��̼� ������ ���� ���� ��� ���·�
		a_pMonster->GetFSM()->ChangeState( WaitInterPolation::GetInstance() );
	}
}


VOID Stiffen::Exit( CMonster* a_pMonster )
{

}