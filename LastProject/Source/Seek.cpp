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
//	// ���Ϳ� ���� �����̿� �ִ� ������ ã�´�.
//	for( INT i=0; i<4; ++i )
//	{
//		// ���� ��ġ - ���� ����
//		temp = UserPos - BossPos;
//	
//		// �Ÿ� ���ϱ�
//		length = D3DXVec3Length( temp );
//
//		if( i == 0 )
//		{
//			min = length;
//			wIndex = i;
//		}
//		else
//		{
//			// �� ������ ������
//			if( length < min )
//			{
//				min = length;
//				wIndex = i;
//			}
//		}
//	}
//
//	// ���� �����̿� �ִ� ������ ���� ������ ������ ���� ���·� ��ȯ
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