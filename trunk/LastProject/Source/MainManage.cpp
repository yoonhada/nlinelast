#include "Stdafx.h"

#include "Monster.h"
#include "BillBoard.h"
#include "Charactor.h"
#include "Camera.h"

CMainManage::CMainManage()
{
	Clear();
}

CMainManage::~CMainManage()
{
	Release();
}

VOID	CMainManage::Clear()
{
	m_pD3dDevice = NULL;
	m_pBill = NULL;
	m_iMaxCharaNum = 4;
	m_bHost = FALSE;
}

HRESULT CMainManage::Create( LPDIRECT3DDEVICE9 a_pD3dDevice )
{
	m_pD3dDevice = a_pD3dDevice;

	m_pMatrices = CMatrices::GetInstance();

	//트리 생성
	CTree::GetInstance()->Create( 1000.0f, 4 );

	//카메라 생성
	m_pCamera = new CCamera;
	m_pCamera->Create( m_pD3dDevice );

	//그리드 생성
	m_pGrid = new CGrid;
	m_pGrid->Create( m_pD3dDevice );

	//캐릭터 생성
	CreateCharactor();

	//몬스터 생성
	m_pMonster = new CMonster;
	m_pMonster->Create( m_pD3dDevice, L"Data/CharData/testNew" );

	//조명 생성
	m_pLight = new CLight;
	m_pLight->Create( m_pD3dDevice );

	//맵 생성
	m_pMap = new Map( m_pD3dDevice );
	m_pMap->Create( L"ASE File/Stage4_1011.ASE", L"ASE File/Stage4_1011.BBX" );

	// 프로젝션 설정
	m_pMatrices->SetupProjection();

	return S_OK;
}

HRESULT CMainManage::Release()
{
	SAFE_DELETE ( m_pCamera );

	SAFE_DELETE ( m_pGrid );

	SAFE_DELETE_ARRAY( m_pCharactors );

	SAFE_DELETE( m_pMonster );

	SAFE_DELETE ( m_pMap );

	SAFE_DELETE ( m_pLight );

	SAFE_DELETE( m_pBill );

	CTree::DestoryInstance();

	return S_OK;
}

CCharactor* CMainManage::CreateObject( INT a_iId )
{
	//IObject* pObject = NULL;
	CCharactor* pCharactor = NULL;
	CMonster* pMonster = NULL;

	switch(a_iId)
	{
	case CHARACTOR:
		pCharactor = new CCharactor;
		pCharactor->Create( m_pD3dDevice, m_pMatrices );
		//pObject = pCharactor;
		break;
	case MONSTER:
		pMonster = new CMonster;
		pMonster->Create( m_pD3dDevice, L"Data/CharData/test" );
		//pObject = pCharactor;
		break;
	default:
		MessageBox( GHWND, L"Error Invalid Class ID", NULL, MB_OK );
	};

	return pCharactor;
}

VOID CMainManage::CreateCharactor()
{
	const INT nChar = 5;
	D3DXVECTOR3 vec[nChar] = { 
		D3DXVECTOR3(  0.0f, 0.0f,  0.0f ), 
		D3DXVECTOR3( 230.0f, 0.0f,  0.0f ), 
		D3DXVECTOR3( 260.0f, 0.0f,  0.0f ), 
		D3DXVECTOR3( 290.0f, 0.0f,  0.0f ), 
		D3DXVECTOR3(  0.0f, 0.0f,100.0f ) 
	};

	//캐릭터 생성
	FLOAT fYawZero = 1.0f;
	m_pCharactors = new CCharactor[m_iMaxCharaNum];
	for(INT Loop=0; Loop<m_iMaxCharaNum; ++Loop )
	{
		m_pCharactors[Loop].Create( m_pD3dDevice, m_pMatrices );
		if(Loop == 0)
			m_pCharactors[Loop].Load( L"Data/CharData/N_0.csav" );
		else if(Loop == 1)
			m_pCharactors[Loop].Load( L"Data/CharData/I_0.csav" );
		else
			m_pCharactors[Loop].Load( L"Data/CharData/N_0.csav" );
		
		if (Loop == 0)	 
			m_pCharactors[Loop].UpdateByInput( );
		else
			m_pCharactors[Loop].Set_Position( vec[Loop] );

		CTree::GetInstance()->GetChaVector()->push_back( m_pCharactors[Loop].GetBoundBox() );

		//D3DXVECTOR3 vec1 = m_pCharactors[Loop].GetBoundBox()->GetPosition();
	}

	// 인풋 업데이트
	FLOAT fMoveSpeed = 50.0f;
	FLOAT fRotateSpeed = 150.0f; 
	CInput::GetInstance()->Update( fMoveSpeed, fRotateSpeed, CFrequency::GetInstance()->getFrametime() );


	//////////////////////////////////////////////////////////////////////////

	//const INT nChar = 5;
	//FLOAT fMoveSpeed = 50.0f;
	//FLOAT fRotateSpeed = 150.0f; 

	//D3DXVECTOR3 vec[nChar] = { 
	//	D3DXVECTOR3(  0.0f, 0.0f,  0.0f ), 
	//	D3DXVECTOR3( 30.0f, 0.0f,  0.0f ), 
	//	D3DXVECTOR3( 60.0f, 0.0f,  0.0f ), 
	//	D3DXVECTOR3( 90.0f, 0.0f,  0.0f ), 
	//	D3DXVECTOR3(  0.0f, 0.0f,100.0f ) 
	//};

	//// 인풋 업데이트
	//CInput::GetInstance()->Update( fMoveSpeed, fRotateSpeed, CFrequency::GetInstance()->getFrametime() );

	//// 케릭터 생성
	//FLOAT fYawZero = 1.0f;
	//CCharactor * pObj;
	//for (int i = 0; i < nChar; ++i)
	//{
	//	pObj = CreateObject( i + PAPA );
	//	pObj->Create( m_pD3dDevice, m_pMatrices, L"Data/CharData/15Box.txt" );
	//	if (i == 0)		pObj->UpdateByInput( CInput::GetInstance()->Get_Pos(), CInput::GetInstance()->Get_MouseYRotate() );
	//	else			pObj->UpdateByValue( vec[i], fYawZero + i );
	//	pObj->Update();
	//	m_vector.push_back( pObj );
	//}
}

VOID	CMainManage::Update()
{
	// 맵 로드
	if( CInput::GetInstance()->Get_F9button() == TRUE )
	{
		OPENFILENAME OFN;
		WCHAR lpstrFile[MAX_PATH]=L"";

		memset(&OFN, 0, sizeof(OPENFILENAME));
		OFN.lStructSize = sizeof(OPENFILENAME);
		OFN.hwndOwner=CWinBase::GetInstance()->Get_hWnd();
		OFN.lpstrFilter=TEXT("ASE 파일(*.ASE)\0*.ASE\0");
		OFN.lpstrFile=lpstrFile;
		OFN.nMaxFile=MAX_PATH;
		if (GetSaveFileName(&OFN)!=0)
		{
			WCHAR* ptr = wcstok( lpstrFile, L"." );
			WCHAR Temp[255];
			WCHAR Temp2[255];
			wsprintf( Temp, L"%s.ASE", ptr );
			wsprintf( Temp2, L"%s.BBX", ptr );
			//m_pMap->Release();
			m_pMap->Create( Temp, Temp2 );
			//m_CharEdit.Load( OFN.lpstrFile );
		}
	}

	//인풋 업데이트
	CInput::GetInstance()->Update( 50.0f, 150.0f, CFrequency::GetInstance()->getFrametime() );

	// 캐릭터: 인풋 값 받아오기
	m_pCharactors[0].UpdateByInput( );
	//m_pCharactors[0].UpdateByInput( CInput::GetInstance()->Get_Pos(),  CInput::GetInstance()->Get_MouseYRotate() );
	m_pCharactors[0].Update();

	////CDebugConsole::GetInstance()->Messagef( L"POS : %f %f %f\n", m_pCharactors[0].Get_CharaPos().x, m_pCharactors[0].Get_CharaPos().z, m_pCharactors[0].Get_CharaAngle() );

	//카메라: 캐릭터 위치,각도 받아오기
	m_pCamera->SetView( m_pCharactors[0].Get_CharaPos(), m_pCharactors[0].Get_PreControl(), 10.0f, 50.0f, 
		m_pCharactors[0].Get_CharaAngle(),
		CInput::GetInstance()->Get_MouseXRotate() );

	m_pCamera->CheckObjectCollision( m_pCamera->GetEye() );

	FLOAT fYawZero = 1.0f;

	m_pCharactors[1].UpdateOtherPlayer();

	//for( INT Loop=2; Loop<m_iMaxCharaNum; ++Loop )
	//{		
	//	//다른 플레이어는 값으로 이동
	//	m_pCharactors[Loop].UpdateByValue( D3DXVECTOR3( static_cast<FLOAT>(Loop)*-60.0f, 0.0f, 0.0f ), 1.0f+static_cast<FLOAT>(Loop) );		
	//	m_pCharactors[Loop].Update();
	//}

	static FLOAT TimeElapsed = 0.0f;
	static FLOAT fMonsterRun = 0.0f;
	TimeElapsed += CFrequency::GetInstance()->getFrametime();
	if( TimeElapsed >= 0.1f )
	{
		fMonsterRun = 0.5f * CFrequency::GetInstance()->getFrametime();
		TimeElapsed = 0.0f;
	}

	//m_pMonster->UpdateByValue( CInput::GetInstance()->Get_Pos(), CInput::GetInstance()->Get_MouseYRotate() );
	m_pMonster->UpdateByValue( D3DXVECTOR3(0.0f, 0.0f, 1.0f), fMonsterRun );
	//m_pMonster->UpdateByValue( D3DXVECTOR3( 0.0f, 0.0f, fMonsterRun ), 0.0f );
	m_pMonster->Update();

	if ( CInput::GetInstance()->Get_Lbutton() )
	{

		//m_pMonster->TestBreakCube();


		for( INT Loop=0; Loop<m_iMaxCharaNum; ++Loop )
		{	
#ifdef _DEBUG
			if (Loop == 0 || Loop == 1)
#endif
				m_pCharactors[Loop].TestBreakCubeAll();
		}
		// YOON
		// 빌보드 작업
		if (m_pBill == NULL)
		{
			m_pBill = new CBillBoard( m_pD3dDevice );
			m_pBill->Create();
			m_pBill->SetType( static_cast<INT>( 4 * FastRand2() ) );
			m_pBill->SetLife(200);
		}
	}

	if ( m_pBill )
	{
		if( m_pBill->IsLife())
		{
			D3DXVECTOR3 vec(0, 20, 0);
			m_pBill->SetInverMatrix( *m_pCamera->GetInvView() );
			m_pBill->SetWorldMatirx( *m_pCamera->GetView() );
			m_pBill->SetPosition( m_pCharactors[0].Get_CharaPos() + vec );
			m_pBill->Update();
		}
		else
		{
			delete m_pBill;
			m_pBill = NULL;
		}
	}


	//m_pD3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
	//m_pMap->Update();

	m_pMap->Set_ControlTranslate( 1, -0.5f );
	m_pMap->Calcul_MatWorld();
}

VOID	CMainManage::Render()
{
	m_pLight->EnableLight();

	m_pMatrices->SetupModeltoWorld( m_pMap->Get_MatWorld() );
	m_pMap->Render();

	m_pCharactors[0].Render();
	for( INT Loop=1; Loop<m_iMaxCharaNum; ++Loop )
	{
		m_pCharactors[Loop].Render();
	}

	m_pMonster->Render();

	if( m_pBill && m_pBill->IsLife() )
	{
		m_pBill->Render();
	}

	//m_pMatrices->SetupModeltoWorld( m_pGrid->Get_MatWorld() );
	//m_pGrid->Render();

	
}