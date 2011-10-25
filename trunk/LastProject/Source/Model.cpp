#include "stdafx.h"
#include "Model.h"

#include "CharCube.h"
#include "Qube.h"

CModel::CModel( LPDIRECT3DDEVICE9 _pd3dDevice )
: m_pd3dDevice( _pd3dDevice )
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

	m_iVectorNum = 0;

	D3DXMatrixIdentity( &m_matMultMatrix );
}

HRESULT CModel::Release()
{
	/*for ( m_iterCube = m_vectorCube.begin(); m_iterCube != m_vectorCube.end(); ++m_iterCube )
	{
		SAFE_DELETE( ( *m_iterCube ) );
	}
	m_vectorCube.clear();*/

	for ( m_iterQube = m_vectorQube.begin(); m_iterQube != m_vectorQube.end(); ++m_iterQube )
	{
		SAFE_DELETE( ( *m_iterQube ) );
	}

	if ( m_vectorQube.empty() == FALSE )
	{
		m_vectorQube.clear();
		m_vectorQube.erase( m_vectorQube.begin(), m_vectorQube.end() );
	}
	//m_vectorQube.clear();

	SAFE_RELEASE( m_pTotalIB );
	SAFE_RELEASE( m_pTotalVB );

	CPhysics::DestoryInstance();

	return S_OK;
}

HRESULT CModel::Create()
{
	//Release();

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

	CPhysics::GetInstance();

	m_vectorQube.reserve(1000);

	return S_OK;
}

VOID CModel::Update()
{
}

VOID CModel::Render()
{
	//// Cube ť�� 
	//m_iterCube = m_vectorCube.begin();
	//while( m_iterCube != m_vectorCube.end() )
	//{
	//	if ( ( *m_iterCube )->GetVisiable() )
	//	{
	//		( *m_iterCube )->Update();
	//		( *m_iterCube )->Draw();
	//	}

	//	m_iterCube++;
	//}

	m_pd3dDevice->SetStreamSource( 0, m_pTotalVB, 0, sizeof( CCube::CUBEVERTEX ) );
	m_pd3dDevice->SetFVF( CCube::CUBEVERTEX::FVF );
	m_pd3dDevice->SetIndices( m_pTotalIB );
	//m_pd3dDevice->SetTexture( 0, m_pTexture );
	//m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//m_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	// Qube ó��
	m_iterQube = m_vectorQube.begin();
	while( m_iterQube != m_vectorQube.end() )
	{
		if ( ( *m_iterQube ) != NULL && ( *m_iterQube )->GetVisiable() )
		{
			//CTree::GetInstance()->InsertObject( CTree::GetInstance()->GetRoot(), ( *m_iterQube ) );
			( *m_iterQube )->Update();
			//( *m_iterQube )->Calcul_MatWorld();
			//D3DXMatrixMultiply( &m_matMultMatrix, &( *m_iterQube )->Get_MatWorld(), &( *m_iterQube )->GetCharMatrix()  );
			m_pd3dDevice->SetTransform( D3DTS_WORLD, &(*m_iterQube )->Get_MatWorld() );
			//CMatrices::GetInstance()->SetupModeltoWorld( ( *m_iterQube )->Get_MatWorld() );
			( *m_iterQube )->Render();
		}
		else 
		{
			//CTree::GetInstance()->RemoveObject( CTree::GetInstance()->GetRoot(), (*m_iterQube) );
			delete ( *m_iterQube );
			m_iterQube = m_vectorQube.erase( m_iterQube );
			continue;
		}

		m_iterQube++;
	}
}

VOID CModel::CreateRandom( CCharCube* a_pCube, INT a_iFrameNum, const D3DXMATRIXA16& a_matChara, const FLOAT fPow )
{
	INT nMax = 0;
	DWORD dwColor = 0xFF000000;
	DWORD dwOutLine1 = 0xFF0000FF;
	DWORD dwOutLine2 = 0xFF000000;
	//CCube * pCube = NULL;
	CQube * pQube = NULL;

	D3DXVECTOR3 vec, vMome;

	D3DXVec3TransformCoord( &vec, &a_pCube->Get_Pos( a_iFrameNum ), &a_matChara );
	//vec = a_pCube->Get_Pos( a_iFrameNum );
	dwColor = a_pCube->Get_Color( a_iFrameNum );

	vMome = D3DXVECTOR3( FastRand2() - 0.5f, FastRand2() - 0.5f, FastRand2() - 0.5f );

	pQube = new CQube;
	//pQube->SetCharMatrix( a_matChara );
	//pQube->SetHeight( a_matChara._42 );
	pQube->SetPosition( vec );
	//vec = a_pCube->Get_Pos( a_iFrameNum );
	//D3DXVec3Normalize(&vec, &vec);
	pQube->RandMome( vMome * fPow );
	pQube->RanderRotate();
	pQube->Create( m_pd3dDevice, m_pTotalVB, m_pTotalIB, 0, 0, 0.5f );
	pQube->InitTexture( dwColor, dwOutLine1 );

	m_vectorQube.push_back( pQube );
}