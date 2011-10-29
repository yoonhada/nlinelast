/**
	@file	Charactor.h
	@date	2011/09/20
	@author	�����
	@brief	ĳ���� Ŭ����
*/

#ifndef _CHARECTOR_H_
#define _CHARECTOR_H_

//using namespace tbb;

class CCharCube;
class CModel;
class CWeapon;
class CShadowCell;

class OctTree;

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
	CCharactor( LPDIRECT3DDEVICE9 a_pD3dDevice, CMatrices* a_pMatrices );
	~CCharactor();

	VOID Clear();
	HRESULT Create();
	HRESULT Create( LPDIRECT3DDEVICE9 a_pD3dDevice, CMatrices* a_pMatrices );
	VOID CreateWeapon( INT nType );

	HRESULT Release();
	VOID Update();
	VOID UpdateByInput( );
	//VOID UpdateByInput( D3DXVECTOR3& a_vControl, FLOAT a_fAngle );
	VOID UpdateByValue( D3DXVECTOR3& a_vControl, FLOAT a_fAngle );
	VOID UpdateOtherPlayer();
	VOID Render();

	VOID World2Model(D3DXVECTOR3& _vPosition);
	VOID BreakCube(D3DXVECTOR3& _vPosition);
	VOID TestBreakCube();
	VOID TestBreakCubeAll();

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
	BOOL Collision();
	VOID Load( WCHAR* a_pFileName );

	// 
	const D3DXVECTOR3& Get_PreControl()
	{
		return m_vPreControl;
	}

	// �ʱ� ��ġ ���� ����
	VOID Set_Position( const D3DXVECTOR3& a_vControl )
	{
		m_vControl = m_vPreControl = a_vControl;
	}

	VOID Set_ClientNumber( WORD a_iClientNumber )
	{
		m_iClientNumber = a_iClientNumber;
	}

	WORD Get_ClientNumber()
	{
		return m_iClientNumber;
	}

	VOID Set_Active( BOOL a_bActive )
	{
		m_bActive = a_bActive;
	}

	const BOOL Get_Active()
	{
		return m_bActive;
	}

private:
	VOID _LoadRelease();
	VOID _SetBoxOutLineColor( CCharCube* a_zCube, DWORD a_dwColor );
	VOID _CreateBase( INT a_iBoxSize, WCHAR* a_pCharaName );
	VOID _CreateAniFrame( INT a_iFrameNum, INT a_iSrcFrameNum = 0, BOOL a_LoadMode = FALSE );
	CCharCube* _CreateCube();
	VOID Animate();
	
	HRESULT InitTexture(  DWORD a_Color, DWORD a_OutLineColor  );

	WORD	m_iClientNumber;
	BOOL m_bActive;

	INT m_iSelectedFrameNum;
	INT m_iMaxCubeCount;
	INT m_iBoxLimit;

	D3DXVECTOR3 m_vColissionControl; ///< ĳ���� �浹�� ��ġ
	D3DXVECTOR3 m_vPreControl; ///< ĳ���� ���� ��ġ ����
	D3DXVECTOR3 m_vControl;  ///< ĳ���� ��ġ
	D3DXVECTOR3 m_vLerpControl; ///< ĳ���� ���� ��ġ
	FLOAT		m_fAngle;	 ///< ĳ���� ����
	FLOAT		m_fNetTime;  ///< ��Ʈ��ũ ���� ��ġ ���� �ð�
	D3DXVECTOR3 m_vFowardVector; ///< ĳ���� ���� �̵� ����
	D3DXVECTOR3 m_vSideStepVector; ///< ĳ���� �¿� �̵� ����

	LPDIRECT3DDEVICE9		m_pD3dDevice; ///< d3d9 ����̽�
	LPDIRECT3DVERTEXBUFFER9 m_pTotalVB;   ///< ť�� ���� ���ؽ�
	LPDIRECT3DINDEXBUFFER9  m_pTotalIB;   ///< ť�� �ε���
	LPDIRECT3DTEXTURE9		m_pTexture;   ///< ť�� ���� �ؽ���

	Object* m_pObject;
	CCharCube* m_pCreateCube;
	CGrid* m_pGrid;
	CModel* m_pModel;

	INT m_iCubeVectorSize;
	INT m_iBoxSize;
	std::vector< CCharCube* > m_vectorCube;
	//std::vector <CCharCube*>::iterator m_iterCube;

	CMatrices*    m_pMatrices;			///< ��� Ŭ���� ������
	D3DXMATRIXA16 m_matMultWorld;		///< ������ ���� ���
	D3DXMATRIXA16 m_matMonster;			///< ���Ϳ� ���
	BOOL		  m_bMatMonster;		///< ���� ��� ���� �Ⱦ��� üũ
	D3DXMATRIXA16 m_matControl;         ///< ���� ���

	INT m_iLoop;
	INT m_iAliveCount;
	BOOL m_bAliveCheck;

	CBoundBox * m_pBoundBox;
	CWeapon * m_pWeapon;
	OctTree * m_pOctTree;

	//�ִϸ��̼� ����
	FLOAT m_fAniAngleY;
	FLOAT m_fAniAngleJump;

private:
	CShadowCell* m_pShadowCell;

};

#endif