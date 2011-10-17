/**
	@file	Charactor.cpp
	@date	2011/09/20
	@author	�����
	@brief	ĳ���� Ŭ����
*/

#include "stdafx.h"
#include "Charactor.h"

CCharactor::CCharactor()
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

	return S_OK;
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

	return S_OK;
}

VOID CCharactor::_LoadRelease()
{

	//���� ť�� ���� �Ҵ� ����
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
	//�ڽ� Ÿ�Կ� ���� �ܰ��� �� �ٸ���
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

	//���̽��� ���ٸ�
	if( NULL == m_pObject  )
	{
		m_iMaxCubeCount = a_iBoxSize * a_iBoxSize * a_iBoxSize;

		m_pObject = new Object;

		lstrcpy( m_pObject->szCharaName, a_pCharaName );

		//�ڽ� ������ ����
		m_iBoxSize = a_iBoxSize;
		m_pObject->iBoxSize = a_iBoxSize;

		//�ڽ� ������ / 2  - 1 (�߰� �ڽ� �� ����)
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
	// �ٿ�� �ڽ� ���Ͽ��� �б�� ����.
	m_pBoundBox = new CBoundBox(this);
	m_pBoundBox->Create();

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

	// ���ؽ�, �ε��� ���� ����
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
	//���� �Ҵ�
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
							//���Ϳ� ������ �� ť�� �ּ� �ֱ�
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

	// ����� �ø��� ���
	//for( INT Loop=0; Loop<m_iCubeVectorSize; ++Loop )
	//{
	//	// �������̽����� �ø��� ���̶��
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

VOID CCharactor::Collision(D3DXVECTOR3& vControl)
{
	// �ɸ��� �̵�
	CBoundBox bbThis;
	bbThis.SetPosition( vControl );
	for ( int i = 0; i < 6; ++i )
		bbThis.SetSize( i, m_pBoundBox->GetSize( i ) );

	std::vector<CBoundBox*> * vecBoundBox = CTree::GetInstance()->GetMapVector(CTree::GetInstance()->GetRoot(), vControl);
	std::vector<CBoundBox*>::iterator Iter;

	BOOL bColl = TRUE;
	if ( vecBoundBox )
	{
		int i = 0;
		for ( Iter = vecBoundBox->begin(); Iter != vecBoundBox->end(); Iter++ )
		{
			if( CPhysics::GetInstance()->Collision( &bbThis, ( *Iter ) ) )
			{
				bColl = FALSE;
				break;
			}
		}
	}

	if ( bColl == TRUE || !CTree::GetInstance()->GetChaVector()->empty() )
	{
		Iter = CTree::GetInstance()->GetChaVector()->begin();
		if( CTree::GetInstance()->GetChaVector()->size() > 0 )
			Iter++;

		for ( ; Iter != CTree::GetInstance()->GetChaVector()->end(); Iter++ )
		{
			if( CPhysics::GetInstance()->Collision( &bbThis, ( *Iter ) ) )
			{
				bColl = FALSE;
				break;
			}
		}
	}

	if ( bColl )
		m_vControl = vControl;

	//CDebugConsole::GetInstance()->Messagef( L"Chara Angle : %f, %f, %f\n", vControl.x, vControl.x, vControl.x );
}

VOID CCharactor::UpdateByInput(  )
{
	D3DXVECTOR3 a_vControl = CInput::GetInstance()->Get_Pos();
	FLOAT a_fAngle = CInput::GetInstance()->Get_MouseYRotate();

	m_fAngle += a_fAngle;
	// 360�� ������ ���� 0���� �ʱ�ȭ
	ABSDEF( m_fAngle ) > 6.2831853f ? m_fAngle = 0.0f : NULL;

	//CDebugConsole::GetInstance()->Messagef( L"Chara Angle : %f\n", m_fAngle );

	D3DXVECTOR3 vControl = m_vControl;
	// ���� ���� ó��
	if( a_vControl.z != 0 )
	{
		D3DXMatrixRotationY( &m_matControl, m_fAngle );
		m_vFowardVector = D3DXVECTOR3(m_matControl._13, 0.0f, -m_matControl._33);
		D3DXVec3Normalize(&m_vFowardVector, &m_vFowardVector);
		vControl += (m_vFowardVector * a_vControl.z);
	}
	// �¿� ó��
	if( a_vControl.x != 0)
	{
		D3DXMatrixRotationY( &m_matControl, m_fAngle + 1.5707963f );
		m_vSideStepVector = D3DXVECTOR3(m_matControl._13, 0.0f, -m_matControl._33);
		D3DXVec3Normalize(&m_vSideStepVector, &m_vSideStepVector);
		vControl += (m_vSideStepVector * a_vControl.x);
	}
	// CDebugConsole::GetInstance()->Messagef( L"Chara ControlX : %f\n", m_vControl.x );
	// CDebugConsole::GetInstance()->Messagef( L"Chara ControlZ : %f\n", m_vControl.z );

	m_vPreControl = vControl;

	Collision(vControl);

	Set_ControlTranslate( 0, m_vControl.x );
	Set_ControlTranslate( 1, m_vControl.y );
	Set_ControlTranslate( 2, m_vControl.z );
	Set_ControlRotate( 1, m_fAngle );
	Calcul_MatWorld();
}

VOID CCharactor::UpdateByInput( D3DXVECTOR3& a_vControl, FLOAT a_fAngle )
{
	
	m_fAngle += a_fAngle;
	// 360�� ������ ���� 0���� �ʱ�ȭ
	if( m_fAngle > 6.2831853 || m_fAngle < -6.2831853 )
	{
		m_fAngle = 0.0f;
	}

	//CDebugConsole::GetInstance()->Messagef( L"Chara Angle : %f\n", m_fAngle );

	
	D3DXVECTOR3 vControl = m_vControl;
	//���� ���� ó��
	if( a_vControl.z != 0 )
	{
		D3DXMatrixRotationY( &m_matControl, m_fAngle );
		m_vFowardVector = D3DXVECTOR3(m_matControl._13, 0.0f, -m_matControl._33);
		D3DXVec3Normalize(&m_vFowardVector, &m_vFowardVector);
		m_vControl += (m_vFowardVector * a_vControl.z);
		vControl += (m_vFowardVector * a_vControl.z);
	}
	//�¿� ó��
	if( a_vControl.x != 0)
	{
		D3DXMatrixRotationY( &m_matControl, m_fAngle + 1.5707963f );
		m_vSideStepVector = D3DXVECTOR3(m_matControl._13, 0.0f, -m_matControl._33);
		D3DXVec3Normalize(&m_vSideStepVector, &m_vSideStepVector);
		m_vControl += (m_vSideStepVector * a_vControl.x);
		vControl += (m_vSideStepVector * a_vControl.x);
	}
	//CDebugConsole::GetInstance()->Messagef( L"Chara ControlX : %f\n", m_vControl.x );
	//CDebugConsole::GetInstance()->Messagef( L"Chara ControlZ : %f\n", m_vControl.z );

	Collision(vControl);

	Set_ControlTranslate( 0, m_vControl.x );
	Set_ControlTranslate( 1, m_vControl.y );
	Set_ControlTranslate( 2, m_vControl.z );
	Set_ControlRotate( 1, m_fAngle );
	Calcul_MatWorld();
	
}

VOID CCharactor::UpdateByValue( D3DXVECTOR3& a_vControl, FLOAT a_fAngle )
{
	m_vPreControl = m_vControl;
	m_vControl = a_vControl;

	m_fAngle = a_fAngle;

	CFrequency::GetInstance()->setTime( 0.0f );
}

VOID CCharactor::UpdateOtherPlayer()
{

	CFrequency::GetInstance()->setPlusTime();
	D3DXVec3Lerp( &m_vLerpControl, &m_vPreControl, &m_vControl, CFrequency::GetInstance()->getTime() / 0.1f );
	//CDebugConsole::GetInstance()->Messagef( L"Lerp Pos: %f %f\n", m_vLerpControl.x, m_vLerpControl.z );
	//CDebugConsole::GetInstance()->Messagef( L"%f\n", CFrequency::GetInstance()->getTime() );

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
}

VOID CCharactor::Update()
{
	//parallel_for( blocked_range<size_t>(0, m_iCubeVectorSize ), MatrixMult( m_vectorCube, Get_MatWorld(), m_iSelectedFrameNum ) );
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
			m_pMatrices->SetupModeltoWorld( m_matMultWorld );
			//m_pMatrices->SetupModeltoWorld( m_vectorCube[Loop]->Get_MatWorld() );
			m_vectorCube[Loop]->Render();
		}
		
	}

	m_pModel->Render();

	m_pD3dDevice->SetTexture( 0, NULL );

	
	

	//a_SetupMatrices.SetupModeltoWorld( m_zGrid.Get_MatWorld() );
	//m_zGrid.Draw();
	
}

VOID CCharactor::TestBreakCube()
{
	

	if( m_bAliveCheck == TRUE )
	{
		if( m_vectorCube[m_iLoop] != NULL &&  m_vectorCube[m_iLoop]->Get_Visible( m_iSelectedFrameNum ) == TRUE
			&&  m_vectorCube[m_iLoop]->Get_Type( m_iSelectedFrameNum ) != EnumCubeType::BONE )
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

	/*if( FAILED( D3DXCreateTextureFromFile( GD3D9DEVICE, L"Media/TerrainTexture/test.bmp", &m_pTexture ) ) )
	{
		return E_FAIL;
	}*/

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
			//�ܰ��� ó��
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