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
/*
	static BOOL bEndAni = FALSE;

	// 애니메이션이 끝날때까지 대기
	static FLOAT fAniTime = 0.0f;
	fAniTime += CFrequency::GetInstance()->getFrametime();
	if( fAniTime >= 4.0f )
	{
		fAniTime = 0.0f;
		bEndAni = TRUE;

		pMonster->Set_iSelectedFrameNum( 0 );
	}

	// 애니메이션이 끝나면 일정시간 경직상태가 된다.
	static FLOAT fTime = 0.0f;
*/
	BOOL bCheck = FALSE;
	if( a_pMonster->Get_AnimationEndCheck() == FALSE )
	{
//		fTime += CFrequency::GetInstance()->getFrametime();
//		if( fTime >= 1.0f )
//		{
//			fTime = 0.0f;
//			bEndAni = FALSE;

			// 애니메이션 0번 복구
			a_pMonster->ChangeAnimation( 0 );

			bCheck = TRUE;

			// 경직상태가 끝나면 애니메이션 보간을 위해 보간 대기 상태로
			a_pMonster->GetFSM()->ChangeState( WaitInterPolation::GetInstance() );
			
//		}
	}

	//if( bCheck == TRUE && pMonster->Get_ChangingAnimation() == FALSE )
	//{
	//	// 경직상태가 끝나면 다시 전투 상태로
	//	pMonster->GetFSM()->ChangeState( Seek::GetInstance() );
	//}
}


VOID Stiffen::Exit( CMonster* a_pMonster )
{

}