/**
	@file	Charactor.cpp
	@date	2011/09/20
	@author	백경훈
	@brief	캐릭터 클래스
*/

#include "stdafx.h"
#include "Charactor.h"

#include "CharCube.h"
#include "Model.h"
#include "Weapon.h"
#include "ShadowCell.h"

CCharactor::CCharactor()
{
	Clear();
}

CCharactor::CCharactor( LPDIRECT3DDEVICE9 a_pD3dDevice, CMatrices* a_pMatrices )
: m_pD3dDevice(a_pD3dDevice)
, m_pMatrices(a_pMatrices)
{
	Clear();
}

CCharactor::~CCharactor()
{
	Release();
}

VOID CCharactor::Clear()
{
	m_iSelectedFrameNum = -1;
	m_iMaxCubeCount = -1;
	m_iBoxLimit = -1;

	m_pTotalVB = NULL;
	m_pTotalIB = NULL;
	m_pTexture = NULL;

	m_pObject = NULL;
	m_pCreateCube = NULL;
	m_pWeapon = NULL;

	m_iCubeVectorSize = -1;
	m_iBoxSize = -1;

	m_iLoop = 0;
	m_iAliveCount = 0;
	m_bAliveCheck = TRUE;

	m_vPreControl = D3DXVECTOR3(200.0f, 0.0f, 0.0f);
	m_vControl = D3DXVECTOR3(200.0f, 0.0f, 0.0f);
	m_vFowardVector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vSideStepVector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fAngle = 0.0f;

	D3DXMatrixIdentity( &m_matMultWorld );
	D3DXMatrixIdentity( &m_matControl );

	m_bMatMonster = FALSE;
	m_iClientNumber = 0;
	m_bActive = FALSE;

	m_fAniAngleY = 0.0f;
}

HRESULT CCharactor::Create()
{
	InitTexture( 0xFFFFFFFF, 0xFF000000 );

	//Load( a_pzFileName );

	//m_pGrid = new CGrid;
	//m_pGrid->Create( m_pD3dDevice );

	m_pModel = new CModel( m_pD3dDevice );

	m_pShadowCell = new CShadowCell;
	m_pShadowCell->Create( m_pD3dDevice, 
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		L"Img/shadow.tga"
		);

	return S_OK;
}

HRESULT CCharactor::Create( LPDIRECT3DDEVICE9 a_pD3dDevice, CMatrices* a_pMatrices )
{
	m_pD3dDevice = a_pD3dDevice;
	m_pMatrices = a_pMatrices;

	InitTexture( 0xFFFFFFFF, 0xFF000000 );

	//Load( a_pzFileName );

	//m_pGrid = new CGrid;
	//m_pGrid->Create( m_pD3dDevice );

	m_pModel = new CModel( m_pD3dDevice );
	
	m_pShadowCell = new CShadowCell;
	m_pShadowCell->Create( m_pD3dDevice, 
						   -1.0f, -1.0f, -1.0f,
						    1.0f, -1.0f, -1.0f,
							1.0f, -1.0f,  1.0f,
						   -1.0f, -1.0f,  1.0f,
							0.0f, 0.0f,
							1.0f, 0.0f,
							1.0f, 1.0f,
							0.0f, 1.0f,
							L"Img/shadow.tga"
						  );
	return S_OK;
}

VOID CCharactor::CreateWeapon( INT nType )
{
	m_pWeapon = new CWeapon( m_pD3dDevice );
	m_pWeapon->SetType( nType );
	m_pWeapon->Create();
}


HRESULT CCharactor::Release()
{
	_LoadRelease();
	//SAFE_DELETE( m_pGrid );

	SAFE_DELETE( m_pModel );
	SAFE_RELEASE( m_pTexture );
	SAFE_RELEASE( m_pTotalVB );
	SAFE_RELEASE( m_pTotalIB );
	SAFE_DELETE( m_pBoundBox );
	SAFE_DELETE( m_pWeapon );
	SAFE_DELETE( m_pShadowCell );

	return S_OK;
}

VOID CCharactor::_LoadRelease()
{

	//벡터 큐브 동적 할당 해제
	for( INT Loop=0; Loop<static_cast<INT>( m_vectorCube.size() ); ++Loop )
	{
		if( m_vectorCube[Loop] != NULL )
		{
			//m_vectorCube[Loop]->Release();
			delete m_vectorCube[Loop];
			m_vectorCube[Loop] = NULL;
		}
	}

	if ( m_vectorCube.empty() == FALSE )
	{
		m_vectorCube.clear();
		m_vectorCube.erase( m_vectorCube.begin(), m_vectorCube.end() );
	}

	if( m_pObject !=NULL )
	{
		delete m_pObject;
		m_pObject=NULL;
	}
}

VOID CCharactor::_SetBoxOutLineColor( CCharCube* a_zCube, DWORD a_dwColor )
{
	//박스 타입에 따라 외곽선 색 다르게
	if ( a_zCube->Get_Type( EnumCharFrame::BASE ) == EnumCubeType::BONE )
	{
		a_zCube->InitTexture( a_dwColor, ConstCubeColor::BONE );
	}
	else if( a_zCube->Get_Type( EnumCharFrame::BASE ) == EnumCubeType::CORE )
	{
		a_zCube->InitTexture( a_dwColor, ConstCubeColor::CORE );
	}
	else
	{
		a_zCube->InitTexture( a_dwColor );
	}
}

VOID CCharactor::_CreateBase( INT a_iBoxSize, WCHAR* a_pCharaName )
{

	m_iSelectedFrameNum = EnumCharFrame::BASE;

	//베이스가 없다면
	if( NULL == m_pObject  )
	{
		m_iMaxCubeCount = a_iBoxSize * a_iBoxSize * a_iBoxSize;

		m_pObject = new Object;

		lstrcpy( m_pObject->szCharaName, a_pCharaName );

		//박스 사이즈 세팅
		m_iBoxSize = a_iBoxSize;
		m_pObject->iBoxSize = a_iBoxSize;

		//박스 사이즈 / 2  - 1 (중간 박스 뺀 길이)
		m_iBoxLimit = (m_pObject->iBoxSize - 1) / 2;
	}
}

VOID CCharactor::_CreateAniFrame( INT a_iFrameNum, INT a_iSrcFrameNum, BOOL a_LoadMode )
{
	m_iSelectedFrameNum = a_iFrameNum;
}


CCharCube* CCharactor::_CreateCube()
{
	m_pCreateCube = NULL;
	m_pCreateCube = new CCharCube;

	return m_pCreateCube;
}


VOID CCharactor::Load( WCHAR* a_pFileName )
{
	// 바운드 박스 파일에서 읽기로 수정.
	m_pBoundBox = new CBoundBox(this);
	m_pBoundBox->Create();
	m_pModel->SetCharType( m_pBoundBox );

	FILE* pFile;

	pFile = _wfopen( a_pFileName, L"r" );

	if( NULL == pFile )
	{
		MessageBox(GHWND, L"File Load Error", a_pFileName, MB_OK);
		return;
	}

	fseek( pFile, 0L, SEEK_SET );

	INT iTemp;
	WCHAR szTemp[255];
	fwscanf( pFile, L"%d", &iTemp );
	fwscanf( pFile, L"%s", szTemp );

	// 버텍스, 인덱스 버퍼 생성
	if( FAILED( m_pD3dDevice->CreateVertexBuffer( CCube::CUBEVERTEX::VertexNum * sizeof( CCube::CUBEVERTEX ),
		0, CCube::CUBEVERTEX::FVF, D3DPOOL_MANAGED, &m_pTotalVB, NULL ) ) )
	{
		MessageBox(GHWND, L"Vertex Buffer Failed", NULL, MB_OK);
	}

	if( FAILED( m_pD3dDevice->CreateIndexBuffer( CCube::CUBEINDEX::IndexNum * sizeof( CCube::CUBEINDEX ), 
		0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pTotalIB, NULL ) ) )
	{
		MessageBox(GHWND, L"Index Buffer Failed", NULL, MB_OK);
	}

	//_CleanUpLoad();
	_CreateBase( iTemp, szTemp );

	fwscanf( pFile, L"%d", &m_pObject->iCubeCount );
	INT iVectorCount=0;
	fwscanf( pFile, L"%d", &iVectorCount );
	//벡터 할당
	m_vectorCube.resize( iVectorCount );

	fwscanf( pFile, L"%d", &iTemp );

	for(INT Loop=0; Loop<EnumCharFrame::MAXFRAME; ++Loop)
	{

		fwscanf( pFile, L"%s", szTemp );
		fwscanf( pFile, L"%d", &iTemp );

		if( iTemp != -1 )
		{
			for(INT xLoop=0; xLoop<m_iBoxSize; ++xLoop)
			{
				for(INT yLoop=0; yLoop<m_iBoxSize; ++yLoop)
				{
					for(INT zLoop=0; zLoop<m_iBoxSize; ++zLoop)
					{
						DWORD dwColor;
						BOOL bVisible;
						D3DXVECTOR3 vPos;
						INT iIndex;
						INT iVectorIndex;
						INT iType;
						INT iFriendVecIndex[6];

						fwscanf( pFile, L"%d %d %d %lx %d %f %f %f %d %d %d %d %d %d",	
							&iIndex,
							&iVectorIndex,
							&iType,
							&dwColor,
							&bVisible,
							&vPos.x,
							&vPos.y,
							&vPos.z,
							&iFriendVecIndex[0],
							&iFriendVecIndex[1],
							&iFriendVecIndex[2],
							&iFriendVecIndex[3],
							&iFriendVecIndex[4],
							&iFriendVecIndex[5]

							);


						if( Loop == EnumCharFrame::BASE )
						{
							//벡터에 생성된 새 큐브 주소 넣기
							if( iVectorIndex != -1 )
							{
								m_vectorCube[ iVectorIndex ] = _CreateCube();

								m_vectorCube[ iVectorIndex ]->Create( m_pD3dDevice, m_pTotalVB, m_pTotalIB, /*(iVectorIndex * CCube::CUBEVERTEX::VertexNum)*/0, 0 );
								m_vectorCube[ iVectorIndex ]->InitTexture( dwColor );
								

							}

						}

						if( iVectorIndex != -1 )
						{
							m_vectorCube[ iVectorIndex ]->Set_NumIndex( Loop, iIndex );
							m_vectorCube[ iVectorIndex ]->Set_Type( Loop, iType );
							m_vectorCube[ iVectorIndex ]->Set_Color( Loop, dwColor );
							m_vectorCube[ iVectorIndex ]->Set_Visible( Loop, bVisible);
							m_vectorCube[ iVectorIndex ]->Set_Pos( Loop, vPos );
							m_vectorCube[ iVectorIndex ]->Set_FriendCubeVecIndex( Loop, 0, iFriendVecIndex[0] );
							m_vectorCube[ iVectorIndex ]->Set_FriendCubeVecIndex( Loop, 1, iFriendVecIndex[1] );
							m_vectorCube[ iVectorIndex ]->Set_FriendCubeVecIndex( Loop, 2, iFriendVecIndex[2] );
							m_vectorCube[ iVectorIndex ]->Set_FriendCubeVecIndex( Loop, 3, iFriendVecIndex[3] );
							m_vectorCube[ iVectorIndex ]->Set_FriendCubeVecIndex( Loop, 4, iFriendVecIndex[4] );
							m_vectorCube[ iVectorIndex ]->Set_FriendCubeVecIndex( Loop, 5, iFriendVecIndex[5] );

							m_vectorCube[iVectorIndex]->Set_ControlTranslate( 0, m_vectorCube[iVectorIndex]->Get_Pos( Loop ).x );
							m_vectorCube[iVectorIndex]->Set_ControlTranslate( 1, m_vectorCube[iVectorIndex]->Get_Pos( Loop ).y );
							m_vectorCube[iVectorIndex]->Set_ControlTranslate( 2, m_vectorCube[iVectorIndex]->Get_Pos( Loop ).z );
							m_vectorCube[iVectorIndex]->Calcul_MatWorld();
							m_vectorCube[iVectorIndex]->Set_Matrix( Loop, m_vectorCube[iVectorIndex]->Get_MatWorld() );
						}
					}
				}
			}
		}

		fwscanf( pFile, L"%s", szTemp );
	}

	fclose( pFile );

	m_iCubeVectorSize = static_cast<INT>( m_vectorCube.size() );

	// 면단위 컬링시 사용
	//for( INT Loop=0; Loop<m_iCubeVectorSize; ++Loop )
	//{
	//	// 인터페이스에서 컬링이 온이라면
	//	if( m_vectorCube[Loop] != NULL && m_vectorCube[Loop]->Get_Visible( m_iSelectedFrameNum ) == TRUE )
	//	{
	//		for(INT LoopCell=0; LoopCell<6; ++LoopCell)
	//		{
	//			if( -1 != m_vectorCube[Loop]->Get_FriendCubeVecIndex( m_iSelectedFrameNum, LoopCell ) )
	//			{
	//				m_vectorCube[Loop]->Set_FaceVisible( LoopCell, FALSE );
	//			}
	//			//else
	//			//{
	//			//	m_vectorCube[Loop]->Set_FaceVisible( LoopCell, TRUE );
	//			//}
	//		}
	//	}
	//}

	//MessageBox(GHWND, L"Load OK", a_pFileName, MB_OK);

	
}

BOOL CCharactor::Collision()
{
	BOOL bColl = FALSE;
	std::vector<CBoundBox*> * vecBoundBox;
	std::vector<CBoundBox*>::iterator Iter;

	INT i;
	D3DXVECTOR3 vDir, vPos;

	// 맵충돌
	for ( i = 0; i < 8; ++i)
	{
		vPos = m_pBoundBox->GetPosition(i);
		vDir = vPos + m_vColissionControl;
		vecBoundBox = CTree::GetInstance()->GetMapVector(CTree::GetInstance()->GetRoot(), vDir);
		if ( !( vecBoundBox == NULL || vecBoundBox->empty() ) )
		{			
			vDir = m_vColissionControl;
			Iter = vecBoundBox->begin();
			while ( Iter != vecBoundBox->end() )
			{
				if( CPhysics::GetInstance()->Collision( vPos, vDir, ( *Iter ) ) )
				{
					CDebugConsole::GetInstance()->Messagef("%f\n", CFrequency::GetInstance()->getFrametime() );
					CPhysics::GetInstance()->Sliding( m_vColissionControl );
					bColl = TRUE;
				}
				Iter++;
			}
		}

		if (bColl)
		{
			break;
		}
	}

	vecBoundBox = CTree::GetInstance()->GetChaVector( );
	if ( vecBoundBox != NULL && vecBoundBox->size() )
	{
		Iter = vecBoundBox->begin();
		Iter++;
		while ( Iter != vecBoundBox->end() )
		{
			if( CPhysics::GetInstance()->Collision( m_pBoundBox, m_vColissionControl, ( *Iter ) ) )
			{
				m_vColissionControl = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				return TRUE;
			}
			Iter++;
		}
	}

	//vecBoundBox = CTree::GetInstance()->GetAtkVector();
	//if ( vecBoundBox != NULL && vecBoundBox->size() )
	//{
	//	Iter = vecBoundBox->begin();
	//	while ( Iter != vecBoundBox->end() )
	//	{
	//		if( CPhysics::GetInstance()->Collision( m_pBoundBox, m_vColissionControl, ( *Iter ) ) )
	//		{
	//			m_vColissionControl = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//			return TRUE;
	//		}

	//		Iter++;
	//	}
	//}
	//vecBoundBox->clear();


	return FALSE;
}

VOID CCharactor::UpdateByInput(  )
{
	D3DXVECTOR3 a_vControl = CInput::GetInstance()->Get_Pos();
	FLOAT a_fAngle = CInput::GetInstance()->Get_MouseYRotate();

	m_fAngle += a_fAngle;
	// 360도 넘으면 라디언 0으로 초기화
	ABSDEF( m_fAngle ) > 6.2831853f ? m_fAngle = 0.0f : NULL;

	////CDebugConsole::GetInstance()->Messagef( L"Chara Angle : %f\n", m_fAngle );

	m_vColissionControl = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//m_vControl;
	// 전진 후진 처리
	if( a_vControl.z != 0 )
	{
		D3DXMatrixRotationY( &m_matControl, m_fAngle );
		m_vFowardVector = D3DXVECTOR3(m_matControl._13, 0.0f, -m_matControl._33);
		D3DXVec3Normalize(&m_vFowardVector, &m_vFowardVector);
		m_vColissionControl = (m_vFowardVector * a_vControl.z);	
	}
	// 좌우 처리
	if( a_vControl.x != 0)
	{
		D3DXMatrixRotationY( &m_matControl, m_fAngle + 1.5707963f );
		m_vSideStepVector = D3DXVECTOR3(m_matControl._13, 0.0f, -m_matControl._33);
		D3DXVec3Normalize(&m_vSideStepVector, &m_vSideStepVector);
		m_vColissionControl += (m_vSideStepVector * a_vControl.x);
	}

	if( a_vControl.z != 0 || a_vControl.x != 0 )
	{
		Animate();
	}
	else
	{
		if( m_fAniAngleY < 0.1f && m_fAniAngleY > -0.1f )
		{
			m_fAniAngleY = 0.0f;
		}
		else
		{
			if( m_fAniAngleY < 0.1f)
			{
				m_fAniAngleY += 3.0f * CFrequency::GetInstance()->getFrametime();
			}
			else if (m_fAniAngleY > -0.1f )
			{
				m_fAniAngleY -= 3.0f * CFrequency::GetInstance()->getFrametime();
			}
		}
	}
	// //CDebugConsole::GetInstance()->Messagef( L"Chara ControlX : %f\n", m_vControl.x );
	// //CDebugConsole::GetInstance()->Messagef( L"Chara ControlZ : %f\n", m_vControl.z );

	//m_vPreControl = vControl;

	m_pBoundBox->SetAngle( m_fAngle );
	Collision();
	m_vPreControl = m_vControl;
	m_vControl += m_vColissionControl;

	Set_ControlTranslate( 0, m_vControl.x );
	Set_ControlTranslate( 1, m_vControl.y );
	Set_ControlTranslate( 2, m_vControl.z );
	Set_ControlRotate( 1, m_fAngle + m_fAniAngleY );
	Calcul_MatWorld();
}

//VOID CCharactor::UpdateByInput( D3DXVECTOR3& a_vControl, FLOAT a_fAngle )
//{
//	
//	m_fAngle += a_fAngle;
//	// 360도 넘으면 라디언 0으로 초기화
//	if( ABSDEF(m_fAngle) > 6.2831853 )
//	{
//		m_fAngle = 0.0f;
//	}
//
//	////CDebugConsole::GetInstance()->Messagef( L"Chara Angle : %f\n", m_fAngle );
//
//	
//	//D3DXVECTOR3 vControl = m_vControl;
//	//전진 후진 처리
//	if( a_vControl.z != 0 )
//	{
//		D3DXMatrixRotationY( &m_matControl, m_fAngle );
//		m_vFowardVector = D3DXVECTOR3(m_matControl._13, 0.0f, -m_matControl._33);
//		D3DXVec3Normalize(&m_vFowardVector, &m_vFowardVector);
//		m_vControl += (m_vFowardVector * a_vControl.z);
//		m_vPreControl += (m_vFowardVector * a_vControl.z);
//	}
//	//좌우 처리
//	if( a_vControl.x != 0)
//	{
//		D3DXMatrixRotationY( &m_matControl, m_fAngle + 1.5707963f );
//		m_vSideStepVector = D3DXVECTOR3(m_matControl._13, 0.0f, -m_matControl._33);
//		D3DXVec3Normalize(&m_vSideStepVector, &m_vSideStepVector);
//		m_vControl += (m_vSideStepVector * a_vControl.x);
//		m_vPreControl += (m_vSideStepVector * a_vControl.x);
//	}
//	////CDebugConsole::GetInstance()->Messagef( L"Chara ControlX : %f\n", m_vControl.x );
//	////CDebugConsole::GetInstance()->Messagef( L"Chara ControlZ : %f\n", m_vControl.z );
//
//	Collision();
//
//	Set_ControlTranslate( 0, m_vControl.x );
//	Set_ControlTranslate( 1, m_vControl.y );
//	Set_ControlTranslate( 2, m_vControl.z );
//	Set_ControlRotate( 1, m_fAngle );
//	Calcul_MatWorld();
//	
//}

VOID CCharactor::UpdateByValue( D3DXVECTOR3& a_vControl, FLOAT a_fAngle )
{
	m_vPreControl = m_vControl;
	m_vControl = a_vControl;

	m_fAngle = a_fAngle;

	m_fNetTime = 0.0f;
	//CFrequency::GetInstance()->setTime( 0.0f );
}

VOID CCharactor::UpdateOtherPlayer()
{

	m_fNetTime += CFrequency::GetInstance()->getFrametime();
	D3DXVec3Lerp( &m_vLerpControl, &m_vPreControl, &m_vControl, m_fNetTime / NETWORK_RECV_TIME );
	////CDebugConsole::GetInstance()->Messagef( L"Lerp Pos: %f %f\n", m_vLerpControl.x, m_vLerpControl.z );
	////CDebugConsole::GetInstance()->Messagef( L"%f\n", CFrequency::GetInstance()->getTime() );

	Set_ControlTranslate( 0, m_vLerpControl.x );
	Set_ControlTranslate( 1, m_vLerpControl.y );
	Set_ControlTranslate( 2, m_vLerpControl.z );
	Set_ControlRotate( 1, m_fAngle );
	Calcul_MatWorld();
}

VOID CCharactor::UpdateMonsterMatrix( const D3DXMATRIXA16& a_matMonster )
{
	m_matMonster = a_matMonster;
	m_bMatMonster = TRUE;

	//D3DXVec3TransformCoord( &m_vPreControl, &m_vPreControl, &Get_MatWorld() );
	D3DXVec3TransformCoord( &m_vPreControl, &m_vPreControl, &m_matMonster );
	//D3DXVec3TransformCoord( &m_vControl, &m_vControl, &Get_MatWorld() );
	D3DXVec3TransformCoord( &m_vControl, &m_vControl, &m_matMonster );
}

VOID CCharactor::Animate()
{
	static BOOL bCheck = FALSE;

	if( m_fAniAngleY < 0.4f && bCheck == FALSE )
	{
		m_fAniAngleY += 3.0f * CFrequency::GetInstance()->getFrametime();
		//m_iSelectedFrameNum = 1;
	}
	else
	{
		bCheck = TRUE;

		if( m_fAniAngleY > -0.4f )
		{
			m_fAniAngleY -= 3.0f * CFrequency::GetInstance()->getFrametime();
			//m_iSelectedFrameNum = 2;
		}
		else
		{
			bCheck = FALSE;
		}
	}

	//CDebugConsole::GetInstance()->Messagef( L"%f \n", m_fAniAngle );
}

VOID CCharactor::Update()
{
	if ( CInput::GetInstance()->Get_Lbutton() )
	{		
		m_pWeapon->SetKeyA( Get_CharaPos() );
	}
	if ( CInput::GetInstance()->Get_Rbutton() )
	{
		m_pWeapon->SetKeyB( Get_CharaPos() );
	}

	if(m_pWeapon)	
		m_pWeapon->Update();
}

VOID CCharactor::Render()
{
	m_pD3dDevice->SetStreamSource( 0, m_pTotalVB, 0, sizeof( CCube::CUBEVERTEX ) );
	m_pD3dDevice->SetFVF( CCube::CUBEVERTEX::FVF );
	m_pD3dDevice->SetIndices( m_pTotalIB );
	m_pD3dDevice->SetTexture( 0, m_pTexture );
	//m_pD3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//m_pD3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//m_pD3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	//m_pD3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	//m_pD3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	for( INT Loop=0; Loop<m_iCubeVectorSize; ++Loop )
	{
		if( m_vectorCube[Loop] != NULL && m_vectorCube[Loop]->Get_Visible( m_iSelectedFrameNum ) == TRUE )
		{
			D3DXMatrixMultiply( &m_matMultWorld, &m_vectorCube[Loop]->Get_Matrix( m_iSelectedFrameNum ), &Get_MatWorld() );
			//m_matMultWorld = m_vectorCube[Loop]->Get_Matrix( m_iSelectedFrameNum ) * Get_MatWorld();
			if( m_bMatMonster )
			{
				D3DXMatrixMultiply( &m_matMultWorld, &m_matMultWorld, &m_matMonster);
			}
			m_pD3dDevice->SetTransform( D3DTS_WORLD, &m_matMultWorld );
			//m_pMatrices->SetupModeltoWorld( m_vectorCube[Loop]->Get_MatWorld() );
			m_vectorCube[Loop]->Render();
		}
		
	}

	m_pModel->Render();
	
	if(m_pWeapon)	
		m_pWeapon->Render( m_pWeapon->Get_MatWorld() * this->Get_MatWorld() );

	m_pShadowCell->Set_ControlScale( 0, 5.0f );
	m_pShadowCell->Set_ControlScale( 2, 5.0f );
	m_pShadowCell->Set_ControlTranslate( 0, m_vControl.x );
	m_pShadowCell->Set_ControlTranslate( 1, 0.51f );
	m_pShadowCell->Set_ControlTranslate( 2, m_vControl.z );
	m_pShadowCell->Calcul_MatWorld();
	m_pD3dDevice->SetTransform( D3DTS_WORLD, &m_pShadowCell->Get_MatWorld() );
	m_pShadowCell->Render();

	m_pD3dDevice->SetTexture( 0, NULL );

	//a_SetupMatrices.SetupModeltoWorld( m_zGrid.Get_MatWorld() );
	//m_zGrid.Draw();	
}

VOID CCharactor::World2Model(D3DXVECTOR3& _vPosition)
{
	D3DXMATRIXA16 matInv = m_pWeapon->Get_MatWorld() * Get_MatWorld();	
	D3DXMatrixInverse( &matInv, NULL, &matInv );
	D3DXVec3TransformCoord( &_vPosition, &_vPosition, &matInv );
}

VOID CCharactor::BreakCube(D3DXVECTOR3& _vPosition)
{
	for( INT Loop = 0; Loop<m_iCubeVectorSize; ++Loop )
	{
		if( m_vectorCube[Loop] == NULL ) 
			continue; 
		D3DXVECTOR3 v = m_vectorCube[Loop]->Get_Pos(m_iSelectedFrameNum);
		if( v == _vPosition && m_vectorCube[Loop]->Get_Visible( m_iSelectedFrameNum ) )
		{
			m_vectorCube[Loop]->Set_Visible( m_iSelectedFrameNum, FALSE );
			m_pModel->CreateRandom( m_vectorCube[Loop], m_iSelectedFrameNum, Get_MatWorld() );
		}
	}
}

VOID CCharactor::TestBreakCubeAll()
{
	if( m_bAliveCheck == TRUE )
	{
		for( INT Loop = 0; Loop<m_iCubeVectorSize; ++Loop )
		{
			if( m_vectorCube[Loop] != NULL && m_vectorCube[Loop]->Get_Type( m_iSelectedFrameNum ) == EnumCubeType::BONE )
			{
				m_vectorCube[Loop]->Set_Visible( m_iSelectedFrameNum, TRUE );
			}

			if( m_vectorCube[Loop] != NULL && m_vectorCube[Loop]->Get_Type( m_iSelectedFrameNum ) != EnumCubeType::BONE )
			{
				m_vectorCube[Loop]->Set_Visible( m_iSelectedFrameNum, FALSE );

				if( m_bMatMonster )
				{
					D3DXMatrixMultiply( &m_matMultWorld, &Get_MatWorld(), &m_matMonster);
					m_pModel->CreateRandom( m_vectorCube[Loop], m_iSelectedFrameNum, m_matMultWorld );
				}
				else
				{
					m_pModel->CreateRandom( m_vectorCube[Loop], m_iSelectedFrameNum, Get_MatWorld() );
				}
			}
		}
	}

	m_bAliveCheck=FALSE;
}

VOID CCharactor::TestBreakCube()
{
	if( m_bAliveCheck == TRUE )
	{
		if( m_vectorCube[m_iLoop] != NULL &&  
			m_vectorCube[m_iLoop]->Get_Visible( m_iSelectedFrameNum ) == TRUE &&  
			m_vectorCube[m_iLoop]->Get_Type( m_iSelectedFrameNum ) != EnumCubeType::BONE )
		{
			m_vectorCube[m_iLoop]->Set_Visible( m_iSelectedFrameNum, 2 );
			if( m_bMatMonster )
			{
				D3DXMatrixMultiply( &m_matMultWorld, &Get_MatWorld(), &m_matMonster);
				m_pModel->CreateRandom( m_vectorCube[m_iLoop], m_iSelectedFrameNum, m_matMultWorld );
			}
			else
			{
				m_pModel->CreateRandom( m_vectorCube[m_iLoop], m_iSelectedFrameNum, Get_MatWorld() );
			}
			
			for(INT Loop=0; Loop<6; ++Loop)
			{
				INT iFriendCubeVecIndex = m_vectorCube[m_iLoop]->Get_FriendCubeVecIndex( m_iSelectedFrameNum, Loop);
				if ( iFriendCubeVecIndex != -1 )
				{
					if( m_vectorCube[ iFriendCubeVecIndex ] != NULL && m_vectorCube[ iFriendCubeVecIndex ]->Get_Visible( m_iSelectedFrameNum ) == FALSE )
					{
						m_vectorCube[ iFriendCubeVecIndex ]->Set_Visible( m_iSelectedFrameNum, TRUE );
					}
				}
			}

			++m_iAliveCount;
		}

		if( m_iLoop < m_iCubeVectorSize-1 )
		{
			++m_iLoop;
		}
		else
		{
			m_iLoop = 0;
			
			//if( m_iAliveCount == 0 )
			//{
				m_bAliveCheck = FALSE;
			//}

			m_iAliveCount = 0;
		}
	}
	else
	{
		if( m_vectorCube[m_iLoop] != NULL )
		{
			m_vectorCube[m_iLoop]->Set_Visible( m_iSelectedFrameNum, TRUE );
		}

		if( m_iLoop < m_iCubeVectorSize-1 )
		{
			++m_iLoop;
		}
		else
		{
			m_iLoop = 0;
			m_iAliveCount = 0;
			m_bAliveCheck = TRUE;
		}
	}
	
}

HRESULT CCharactor::InitTexture( DWORD a_Color, DWORD a_OutLineColor )
{

	LPBYTE pData;	
	LPDWORD pDWord;
	D3DLOCKED_RECT Texture_Locked;
	INT TextureSize = 24;


	if( m_pTexture != NULL )
	{
		m_pTexture->Release();
		m_pTexture=NULL;
	}
	if( FAILED( m_pD3dDevice->CreateTexture( TextureSize, TextureSize, 1, 0, D3DFMT_A8R8G8B8, 
		D3DPOOL_MANAGED, &m_pTexture, NULL ) ) )
	{	
		return E_FAIL;	
	}

	memset( &Texture_Locked, 0, sizeof(D3DLOCKED_RECT) );
	if( FAILED(m_pTexture->LockRect(0, &Texture_Locked, NULL, 0)) )
	{	
		return E_FAIL;	
	}

	pData = (LPBYTE)Texture_Locked.pBits;
	for(INT iLoopY=0; iLoopY<TextureSize; ++iLoopY)
	{
		pDWord = LPDWORD(pData + iLoopY * Texture_Locked.Pitch);
		for(INT iLoopX=0; iLoopX<TextureSize; ++iLoopX)
		{	
			//외곽선 처리
			if( iLoopX == 0 || iLoopX == TextureSize-1 || iLoopY == 0 || iLoopY == TextureSize-1 )
			{
				*(pDWord + iLoopX) = a_OutLineColor;
			}
			else
			{
				*(pDWord + iLoopX) = a_Color;
			}
		}
	}

	if( FAILED(m_pTexture->UnlockRect(0)) )	
	{	
		return E_FAIL;	
	}

	return S_OK;
}