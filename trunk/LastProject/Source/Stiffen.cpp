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
	static FLOAT t = 0.0f;
	t += CFrequency::GetInstance()->getFrametime();

	if( t >= 1.5f )
	{
		t = 0.0f;

		// �ִϸ��̼� 0�� ����
		a_pMonster->ChangeAnimation( 0 );

		// �������°� ������ �ִϸ��̼� ������ ���� ���� ��� ���·�
		a_pMonster->GetFSM()->ChangeState( WaitInterPolation::GetInstance() );
	}
}


VOID Stiffen::Exit( CMonster* a_pMonster )
{

}