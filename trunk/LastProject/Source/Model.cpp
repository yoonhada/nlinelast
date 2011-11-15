#include "stdafx.h"
#include "Model.h"

#include "CharCube.h"
#include "Qube.h"

CModel::CModel( LPDIRECT3DDEVICE9 _pd3dDevice )
: m_pd3dDevice( _pd3dDevice )
, m_pParentBB( NULL )
{
	Create();
}

CModel::~CModel()
{
	Release();
}

VOID CModel::Clear()
{
	m_pd3dDevice = NULL;
	m_pTotalVB = NULL;
	m_pTotalIB = NULL;
	m_pParentBB = NULL;

	m_iVectorNum = 0;

	D3DXMatrixIdentity( &m_matMultMatrix );
}

HRESULT CModel::Release()
{
	for ( m_iterQube = m_vectorQube.begin(); m_iterQube != m_vectorQube.end(); ++m_iterQube )
	{
		SAFE_DELETE( ( *m_iterQube ) );
	}

	if ( m_vectorQube.empty() == FALSE )
	{
		m_vectorQube.clear();
		m_vectorQube.erase( m_vectorQube.begin(), m_vectorQube.end() );
	}

	SAFE_RELEASE( m_pTotalIB );
	SAFE_RELEASE( m_pTotalVB );

	return S_OK;
}

HRESULT CModel::Create()
{
	if( FAILED( m_pd3dDevice->CreateVertexBuffer( CCube::CUBEVERTEX::VertexNum * sizeof( CCube::CUBEVERTEX ),
		0, CCube::CUBEVERTEX::FVF, D3DPOOL_MANAGED, &m_pTotalVB, NULL ) ) )
	{
		MessageBox(GHWND, L"Vertex Buffer Failed", NULL, MB_OK);
	}

	if( FAILED( m_pd3dDevice->CreateIndexBuffer( CCube::CUBEINDEX::IndexNum * sizeof( CCube::CUBEINDEX ), 
		0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pTotalIB, NULL ) ) )
	{
		MessageBox(GHWND, L"Index Buffer Failed", NULL, MB_OK);
	}

	m_vectorQube.reserve(1000);

	return S_OK;
}

VOID CModel::Update()
{
	Delete();
	Move();
}

VOID CModel::Move()
{
	m_hEvent[0] = ::CreateEvent(NULL, TRUE, FALSE, L"UpdateThread0"); 
	m_hEvent[1] = ::CreateEvent(NULL, TRUE, FALSE, L"UpdateThread1"); 
	m_hEvent[2] = ::CreateEvent(NULL, TRUE, FALSE, L"UpdateThread2"); 
	m_hEvent[3] = ::CreateEvent(NULL, TRUE, FALSE, L"UpdateThread3"); 
	
	_beginthreadex(NULL, 0, UpdateThread0, (LPVOID)this, 0, NULL);
	_beginthreadex(NULL, 0, UpdateThread1, (LPVOID)this, 0, NULL);
	_beginthreadex(NULL, 0, UpdateThread2, (LPVOID)this, 0, NULL);
	_beginthreadex(NULL, 0, UpdateThread3, (LPVOID)this, 0, NULL);

	SetEvent(m_hEvent[0]);
	SetEvent(m_hEvent[1]);
	SetEvent(m_hEvent[2]);
	SetEvent(m_hEvent[3]);
	
	WaitForSingleObject( m_hEvent[0], INFINITE );
	WaitForSingleObject( m_hEvent[1], INFINITE );
	WaitForSingleObject( m_hEvent[2], INFINITE );
	WaitForSingleObject( m_hEvent[3], INFINITE );
}

UINT CModel::UpdateThread( INT nBegin, INT nEnd )
{
	CQube * pQube;
	for (int i = nBegin; i < nEnd; ++i)
	{
		pQube = m_vectorQube[i];
		if ( pQube != NULL && pQube->GetVisiable() )
		{
			pQube->Update( m_pParentBB );
		}
	}

	return 0;
}

UINT CModel::UpdateThread0(LPVOID lp)
{
	CModel *pThis = static_cast<CModel *>(lp);

	INT nIndexBegin = 0;
	INT nIndexEnd   = pThis->m_vectorQube.size() / 4;

	pThis->UpdateThread( nIndexBegin, nIndexEnd );

	return 0;
}

UINT CModel::UpdateThread1(LPVOID lp)
{
	CModel *pThis = static_cast<CModel *>(lp);

	INT nIndexBegin = pThis->m_vectorQube.size() / 4;
	INT nIndexEnd   = pThis->m_vectorQube.size() / 2;

	pThis->UpdateThread( nIndexBegin, nIndexEnd );

	return 0;
}

UINT CModel::UpdateThread2(LPVOID lp)
{
	CModel *pThis = static_cast<CModel *>(lp);

	INT nIndexBegin = pThis->m_vectorQube.size() / 2;
	INT nIndexEnd   = pThis->m_vectorQube.size() / 4 * 3;

	pThis->UpdateThread( nIndexBegin, nIndexEnd );

	return 0;
}

UINT CModel::UpdateThread3(LPVOID lp)
{
	CModel *pThis = static_cast<CModel *>(lp);

	INT nIndexBegin = pThis->m_vectorQube.size() / 4 * 3;
	INT nIndexEnd   = pThis->m_vectorQube.size();

	pThis->UpdateThread( nIndexBegin, nIndexEnd );

	return 0;
}

VOID CModel::Delete()
{
	std::vector<CQube*>::iterator Iter;
	Iter = m_vectorQube.begin();
	while( Iter != m_vectorQube.end() )
	{
		if ( ( *Iter ) != NULL && !( *Iter )->GetVisiable() )
		{
			delete ( *Iter );
			Iter = m_vectorQube.erase( Iter );
			continue;
		}

		Iter++;
	}
}

VOID CModel::Render()
{
	m_pd3dDevice->SetStreamSource( 0, m_pTotalVB, 0, sizeof( CCube::CUBEVERTEX ) );
	m_pd3dDevice->SetFVF( CCube::CUBEVERTEX::FVF );
	m_pd3dDevice->SetIndices( m_pTotalIB );

	// Qube o??
	m_iterQube = m_vectorQube.begin();
	while( m_iterQube != m_vectorQube.end() )
	{
		if ( ( *m_iterQube ) != NULL && ( *m_iterQube )->GetVisiable() )
		{
			( *m_iterQube )->Update( m_pParentBB );
			m_pd3dDevice->SetTransform( D3DTS_WORLD, &( *m_iterQube )->Get_MatWorld() );
			( *m_iterQube )->Render();
		}
		else 
		{
			delete ( *m_iterQube );
			m_iterQube = m_vectorQube.erase( m_iterQube );
			continue;
		}

		m_iterQube++;
	}
}

VOID CModel::Render(INT n)
{
	m_pd3dDevice->SetStreamSource( 0, m_pTotalVB, 0, sizeof( CCube::CUBEVERTEX ) );
	m_pd3dDevice->SetFVF( CCube::CUBEVERTEX::FVF );
	m_pd3dDevice->SetIndices( m_pTotalIB );

	// Qube o??
	m_iterQube = m_vectorQube.begin();
	while( m_iterQube != m_vectorQube.end() )
	{
		if ( ( *m_iterQube ) != NULL && ( *m_iterQube )->GetVisiable() )
		{
			m_pd3dDevice->SetTransform( D3DTS_WORLD, &( *m_iterQube )->Get_MatWorld() );
			( *m_iterQube )->Render();
		}

		m_iterQube++;
	}
}

VOID CModel::CreateRandom( CCharCube* a_pCube, 
						   INT a_iFrameNum, 
						   const D3DXMATRIXA16& a_matChara, 
						   const D3DXVECTOR3& a_vMome, 
						   const FLOAT fPow )
{

	INT nMax = 0;
	DWORD dwColor = 0xFF000000;
	DWORD dwOutLine1 = 0xFF0000FF;
	DWORD dwOutLine2 = 0xFF000000;
	CQube * pQube = NULL;

	D3DXVECTOR3 vec;
	D3DXVECTOR3 vMome = a_vMome;

	D3DXVec3TransformCoord( &vec, &a_pCube->Get_Pos( a_iFrameNum ), &a_matChara );
	dwColor = a_pCube->Get_Color( a_iFrameNum );

	D3DXMATRIXA16 mat = a_matChara;
	mat._41 = 0.0f;
	mat._42 = 0.0f;
	mat._43 = 0.0f;

	pQube = new CQube;
	pQube->SetPosition( vec );
	pQube->RandMome( vMome, fPow );
	pQube->RanderRotate();
	pQube->Create( m_pd3dDevice, m_pTotalVB, m_pTotalIB, 0, 0, 0.5f );
	pQube->InitTexture( dwColor, dwOutLine1 );

	m_vectorQube.push_back( pQube );
}