/**
	@file	Charactor.h
	@date	2011/09/20
	@author	백경훈
	@brief	캐릭터 클래스
*/

#ifndef _CHARECTOR_H_
#define _CHARECTOR_H_

//using namespace tbb;

#include "CharCube.h"
#include "Model.h"

//class MatrixMult
//{
//public:
//	MatrixMult( std::vector< CCharCube* > a_pVectorCube, 
//				const D3DXMATRIXA16& a_matChara, 
//				const INT a_iSelectedFrame ) : m_pVectorCube(a_pVectorCube)
//				,m_matChara(a_matChara)
//				,m_iSelectedFrame(a_iSelectedFrame) {}
//
//public:
//	void operator()( const blocked_range<size_t>& r ) const
//	{
//		for( size_t i=r.begin(); i!=r.end(); ++i )
//		{
//			if( m_pVectorCube[i] != NULL )
//			{
//				m_pVectorCube[i]->Set_Matrix( m_iSelectedFrame, m_pVectorCube[i]->Get_Matrix( m_iSelectedFrame ) * m_matChara ) ;
//			}
//		}
//	}
//
//private:
//	D3DXMATRIXA16 m_matChara;
//	std::vector< CCharCube* > m_pVectorCube;
//	INT m_iSelectedFrame;
//};

/**
	@class	CCharactor
	@date	2011/09/20
	@author	백경훈
	@brief	캐릭터 클래스
*/
class CCharactor : virtual public IObject, public CObjectSRT
{
public:

	struct Object
	{
		WCHAR		szCharaName[255];	///< 캐릭터 이름

		INT			iBoxSize;			///< 박스 사이즈
		INT			iCubeCount;			///< 큐브 갯수

		Object() : iBoxSize(0), iCubeCount(0)
		{
			ZeroMemory( szCharaName, sizeof(szCharaName) );
		}
		~Object() {}
	};

	CCharactor();
	~CCharactor();

	VOID Clear();
	HRESULT Create() { return S_OK; }
	HRESULT Create( LPDIRECT3DDEVICE9 a_pD3dDevice, CMatrices* a_pMatrices );
	HRESULT Release();
	VOID Update();
	VOID UpdateByInput( );
	VOID UpdateByInput( D3DXVECTOR3& a_vControl, FLOAT a_fAngle );
	VOID UpdateByValue( D3DXVECTOR3& a_vControl, FLOAT a_fAngle );
	VOID Render();

	VOID TestBreakCube();

	const D3DXVECTOR3& Get_CharaPos()
	{
		return m_vControl;
	}

	const FLOAT Get_CharaAngle()
	{
		return m_fAngle;
	}

	VOID UpdateMonsterMatrix( const D3DXMATRIXA16& a_matMonster );

	CBoundBox * GetBoundBox()			{ return m_pBoundBox; }
	VOID Collision(D3DXVECTOR3& vControl);
	VOID Load( WCHAR* a_pFileName );

	const D3DXVECTOR3& Get_PreControl()
	{
		return m_vPreControl;
	}


private:
	VOID _LoadRelease();
	VOID _SetBoxOutLineColor( CCharCube* a_zCube, DWORD a_dwColor );
	VOID _CreateBase( INT a_iBoxSize, WCHAR* a_pCharaName );
	VOID _CreateAniFrame( INT a_iFrameNum, INT a_iSrcFrameNum = EnumCharFrame::BASE, BOOL a_LoadMode = FALSE );
	CCharCube* _CreateCube();
	
	HRESULT InitTexture(  DWORD a_Color, DWORD a_OutLineColor  );

	INT m_iSelectedFrameNum;
	INT m_iMaxCubeCount;
	INT m_iBoxLimit;

	D3DXVECTOR3 m_vPreControl; ///< 캐릭터 이전 위치 저장
	D3DXVECTOR3 m_vControl;  ///< 캐릭터 위치
	FLOAT		m_fAngle;	 ///< 캐릭터 각도
	D3DXVECTOR3 m_vFowardVector; /// 캐릭터 전진 이동 벡터
	D3DXVECTOR3 m_vSideStepVector; /// 캐릭터 좌우 이동 벡터

	LPDIRECT3DDEVICE9		m_pD3dDevice; ///< d3d9 디바이스
	LPDIRECT3DVERTEXBUFFER9 m_pTotalVB;   ///< 큐브 통합 버텍스
	LPDIRECT3DINDEXBUFFER9  m_pTotalIB;   ///< 큐브 인덱스
	LPDIRECT3DTEXTURE9		m_pTexture;

	Object* m_pObject;
	CCharCube* m_pCreateCube;
	CGrid* m_pGrid;
	CModel* m_pModel;

	INT m_iCubeVectorSize;
	INT m_iBoxSize;
	std::vector< CCharCube* > m_vectorCube;
	//std::vector <CCharCube*>::iterator m_iterCube;

	CMatrices* m_pMatrices;				///< 행렬 클래스 포인터
	D3DXMATRIXA16 m_matMultWorld;		///< 적용할 최종 행렬
	D3DXMATRIXA16 m_matMonster;			///< 몬스터용 행렬
	BOOL		  m_bMatMonster;
	D3DXMATRIXA16 m_matControl;         ///< 조작 행렬

	INT m_iLoop;
	INT m_iAliveCount;
	BOOL m_bAliveCheck;

	CBoundBox * m_pBoundBox;
};

#endif