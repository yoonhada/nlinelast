/**
	@file	Charactor.h
	@date	2011/09/20
	@author	백경훈
	@brief	캐릭터 클래스
*/

#ifndef _CHARECTOR_H_
#define _CHARECTOR_H_

#include <list>
//using namespace tbb;

class CCharCube;
class CModel;
class CWeapon;
class CShadowCell;

class COctTree2Array;

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
	//CCharactor( LPDIRECT3DDEVICE9 a_pD3dDevice );
	virtual ~CCharactor();

	VOID Clear();
	HRESULT Create() { return S_OK; }
	HRESULT Create( LPDIRECT3DDEVICE9 a_pD3dDevice );
	VOID CreateWeapon( INT nType );

	HRESULT Release();
	VOID Update();
	VOID UpdateByInput( );
	//VOID UpdateByInput( D3DXVECTOR3& a_vControl, FLOAT a_fAngle );
	VOID UpdateByValue( D3DXVECTOR3& a_vControl, FLOAT a_fAngle );
	VOID UpdateOtherPlayer();
	VOID UpdateOtherPlayer(BOOL);
	VOID Render();

	VOID World2Model(D3DXVECTOR3& _vPosition);
	VOID BreakCube(D3DXVECTOR3& _vPosition);
	VOID BreakQube(D3DXMATRIXA16 &mat);
	VOID TestBreakCube();
	VOID BreakCubeAll();
	VOID BreakListMake(INT, D3DXVECTOR3&);

	const D3DXVECTOR3& Get_CharaPos()
	{
		return m_vControl;
	}

	const FLOAT Get_CharaAngle()
	{
		return m_fAngle;
	}

	const D3DXVECTOR3& Get_CharaPos2Camera();
	const D3DXVECTOR3& Get_PreCharaPos2Camera();

	VOID UpdateMonsterPos( const D3DXVECTOR3& a_vPrePos, const D3DXVECTOR3& a_vPos, const FLOAT a_fAngle );
	VOID UpdateMonsterMatrix( const D3DXMATRIXA16& a_matMonster );

	CBoundBox * GetBoundBox()			{ return m_pBoundBox; }
	BOOL Collision( D3DXVECTOR3& a_vCollisionControl );
	BOOL CollisionAtk( );
	VOID Load( WCHAR* a_pFileName );

	// 
	const D3DXVECTOR3& Get_PreControl()
	{
		return m_vPreControl;
	}

	// 초기 위치 강제 세팅
	VOID Set_Position( const D3DXVECTOR3& a_vControl )
	{
		m_vControl = m_vPreControl = a_vControl;
	}

	VOID Set_Angle( FLOAT a_fAngle )				
	{ 
		m_fAngle = a_fAngle; 
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

	const BOOL Get_bMonster()
	{
		return m_bMonster;
	}

	VOID Set_WeaponAnimationState( const WORD a_iNumber );

	const INT Get_WeaponAnimationState();

	VOID RecvBreakList( INT a_iCount, WORD* a_pList, D3DXVECTOR3& a_vDir );

	VOID Set_MonsterPart( CHAR a_chMonsterPart )
	{
		m_chMonsterPart = a_chMonsterPart;
	}

	VOID LoadKindChar( INT nKind );

private:
	VOID _LoadRelease();
	VOID _SetBoxOutLineColor( CCharCube* a_zCube, DWORD a_dwColor );
	VOID _CreateBase( INT a_iBoxSize, WCHAR* a_pCharaName );
	VOID _CreateAniFrame( INT a_iFrameNum, INT a_iSrcFrameNum = 0, BOOL a_LoadMode = FALSE );
	CCharCube* _CreateCube();
	VOID AnimateMove(BOOL);
	FLOAT AnimateAttack();
	
	HRESULT InitTexture(  DWORD a_Color, DWORD a_OutLineColor  );

	WORD m_iClientNumber;
	BOOL m_bActive;
	CHAR m_chMonsterPart; ///< 몬스터 어느 부위인지

	INT m_iSelectedFrameNum;
	INT m_iMaxCubeCount;
	INT m_iBoxLimit;

	D3DXVECTOR3 m_vPreControl2Camera; ///< 카메라 오른쪽 보내기용 이전 위치
	D3DXVECTOR3 m_vControl2Camera;	  ///< 카메라 오른쪽 보내기용 위치
	D3DXVECTOR3 m_vColissionControl; ///< 캐릭터 충돌용 위치
	D3DXVECTOR3 m_vPreControl; ///< 캐릭터 이전 위치 저장
	D3DXVECTOR3 m_vControl;  ///< 캐릭터 위치
	D3DXVECTOR3 m_vLerpControl; ///< 캐릭터 보간 위치
	FLOAT		m_fPreAngle; ///< 캐릭터 이전 각도
	FLOAT		m_fAngle;	 ///< 캐릭터 각도
	FLOAT		m_fNetTime;  ///< 네트워크 전송 위치 보정 시간
	D3DXVECTOR3 m_vFowardVector; ///< 캐릭터 전진 이동 벡터
	D3DXVECTOR3 m_vSideStepVector; ///< 캐릭터 좌우 이동 벡터
	//D3DXVECTOR3 m_vKnockBack;

	LPDIRECT3DDEVICE9		m_pD3dDevice; ///< d3d9 디바이스
	LPDIRECT3DVERTEXBUFFER9 m_pTotalVB;   ///< 큐브 통합 버텍스
	LPDIRECT3DINDEXBUFFER9  m_pTotalIB;   ///< 큐브 인덱스
	LPDIRECT3DTEXTURE9		m_pTexture;   ///< 큐브 통합 텍스쳐

	Object* m_pObject;
	CCharCube* m_pCreateCube;
	CGrid* m_pGrid;
	CModel* m_pModel;

	INT m_iCubeVectorSize;
	INT m_iBoxSize;
	std::vector< CCharCube* > m_vectorCube;
	//std::vector <CCharCube*>::iterator m_iterCube;

	D3DXMATRIXA16 m_matMultWorld;		///< 적용할 최종 행렬
	D3DXMATRIXA16 m_matMonster;			///< 몬스터용 행렬
	BOOL		  m_bMonster;			///< 몬스터인지 아닌지 체크
	D3DXMATRIXA16 m_matControl;         ///< 조작 행렬

	INT m_iLoop;
	INT m_iAliveCount;
	BOOL m_bAliveCheck;

	CBoundBox * m_pBoundBox;
	CWeapon * m_pWeapon;
	COctTree2Array * m_pOctTree;

	//애니메이션 변수
	INT m_nAniAttackFrame;
	FLOAT m_fKnockBack;
	FLOAT m_fTransition;
	FLOAT m_fAniAngleAttack;
	FLOAT m_fAniAngle;
	D3DXVECTOR3 m_vAniVector;
	const FLOAT fAniAngleTurn;
	const FLOAT fAniAngleLimit;
	const FLOAT fAniMagicNum;

private:
	BOOL m_bShadowEnable;
	CShadowCell* m_pShadowCell;

public:
	VOID EnableShadow( BOOL bEnable )	{ m_bShadowEnable = bEnable; }
	BOOL AliveCheck(BOOL bState = -1);

};

#endif