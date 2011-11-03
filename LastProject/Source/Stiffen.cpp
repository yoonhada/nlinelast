#include "stdafx.h"

#include "Stiffen.h"
#include "Battle.h"
#include "Seek.h"

#include "Monster.h"
#include "Frequency.h"


Stiffen* Stiffen::GetInstance()
{
	static Stiffen Instance;

	return &Instance;
}


VOID Stiffen::Enter( CMonster* pMonster )
{

}


VOID Stiffen::Execute( CMonster* pMonster )
{
/*
	static BOOL bEndAni = FALSE;

	// �ִϸ��̼��� ���������� ���
	static FLOAT fAniTime = 0.0f;
	fAniTime += CFrequency::GetInstance()->getFrametime();
	if( fAniTime >= 4.0f )
	{
		fAniTime = 0.0f;
		bEndAni = TRUE;

		pMonster->Set_iSelectedFrameNum( 0 );
	}

	// �ִϸ��̼��� ������ �����ð� �������°� �ȴ�.
	static FLOAT fTime = 0.0f;
*/
	if( pMonster->Get_AnimationEndCheck() == FALSE )
	{
//		fTime += CFrequency::GetInstance()->getFrametime();
//		if( fTime >= 1.0f )
//		{
//			fTime = 0.0f;
//			bEndAni = FALSE;

			// �ִϸ��̼� 0�� ����
			pMonster->ChangeAnimation( 0 );

			// �������°� ������ �ٽ� ���� ���·�
			pMonster->GetFSM()->ChangeState( Seek::GetInstance() );
//		}
	}
}


VOID Stiffen::Exit( CMonster* pMonster )
{

}