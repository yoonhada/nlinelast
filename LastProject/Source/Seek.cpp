//#include "stdafx.h"
//
//#include "Seek.h"
//
//
//Seek& Seek::GetInstance()
//{
//	static Seek Instance;
//
//	return &Instance;
//}
//
//
//VOID Seek::Enter( Miner* pMiner )
//{
//
//}
//
//
//VOID Seek::Execute( Miner* pMiner )
//{
//	WORD wIndex = 0;
//	D3DXVECTOR3 temp;
//	FLOAT length = 0.0f;
//	FLOAT min = 0.0f;
//
//	// 몬스터와 가장 가까이에 있는 유저를 찾는다.
//	for( INT i=0; i<4; ++i )
//	{
//		// 유저 위치 - 보스 몬스터
//		temp = UserPos - BossPos;
//	
//		// 거리 구하기
//		length = D3DXVec3Length( temp );
//
//		if( i == 0 )
//		{
//			min = length;
//			wIndex = i;
//		}
//		else
//		{
//			// 더 가까이 있으면
//			if( length < min )
//			{
//				min = length;
//				wIndex = i;
//			}
//		}
//	}
//
//	// 가장 가까이에 있는 유저가 공격 범위에 있으면 전투 상태로 전환
//	if( min < Ranger )
//	{
//		pMiner->ChangeState( Battle::GetInstance() );
//	}
//}
//
//
//VOID Seek::Exit( Miner* pMiner )
//{
//
//}