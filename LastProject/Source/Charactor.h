/**
	@file	Charactor.h
	@date	2011/09/20
	@author	�����
	@brief	ĳ���� Ŭ����
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
	@author	�����
	@brief	ĳ���� Ŭ����
*/
class CCharactor : virtual public IObject, public CObjectSRT
{
public:

	struct Object
	{
		WCHAR		szCharaName[255];	///< ĳ���� �̸�

		INT			iBoxSize;			///< �ڽ� ������
		INT			iCubeCount;			///< ť�� ����

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

	D3DXVECTOR3 m_vPreControl; ///< ĳ���� ���� ��ġ ����
	D3DXVECTOR3 m_vControl;  ///< ĳ���� ��ġ
	FLOAT		m_fAngle;	 ///< ĳ���� ����
	D3DXVECTOR3 m_vFowardVector; /// ĳ���� ���� �̵� ����
	D3DXVECTOR3 m_vSideStepVector; /// ĳ���� �¿� �̵� ����

	LPDIRECT3DDEVICE9		m_pD3dDevice; ///< d3d9 ����̽�
	LPDIRECT3DVERTEXBUFFER9 m_pTotalVB;   ///< ť�� ���� ���ؽ�
	LPDIRECT3DINDEXBUFFER9  m_pTotalIB;   ///< ť�� �ε���
	LPDIRECT3DTEXTURE9		m_pTexture;

	Object* m_pObject;
	CCharCube* m_pCreateCube;
	CGrid* m_pGrid;
	CModel* m_pModel;

	INT m_iCubeVectorSize;
	INT m_iBoxSize;
	std::vector< CCharCube* > m_vectorCube;
	//std::vector <CCharCube*>::iterator m_iterCube;

	CMatrices* m_pMatrices;				///< ��� Ŭ���� ������
	D3DXMATRIXA16 m_matMultWorld;		///< ������ ���� ���
	D3DXMATRIXA16 m_matMonster;			///< ���Ϳ� ���
	BOOL		  m_bMatMonster;
	D3DXMATRIXA16 m_matControl;         ///< ���� ���

	INT m_iLoop;
	INT m_iAliveCount;
	BOOL m_bAliveCheck;

	CBoundBox * m_pBoundBox;
};

#endif