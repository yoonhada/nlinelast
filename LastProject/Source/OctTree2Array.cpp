#include "stdafx.h"
#include "OctTree2Array.h"

VOID COctTree2Array::Initialize()
{
	for( INT i=0 ; i<8 ; i++ )
		m_pChild[ i ] = NULL;

	m_nSize = 0;
}

VOID COctTree2Array::Release()
{
	Cleanup();
}

VOID COctTree2Array::Cleanup()
{
	for( INT i=0 ; i<8 ; i++ )
	{
		SAFE_DELETE( m_pChild[i] );
	}
}

VOID COctTree2Array::Build( INT _nSize )
{
	//	CleanupÀ¸·Î ¹Ù²Ü·¡
	Cleanup();

	m_Area.vCenter = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_nSize = _nSize;
	BuildOctTree( );
}

VOID COctTree2Array::BuildOctTree( )
{
	if( SubDivide( ) )
	{
		//	Up
		m_pChild[ LUF ]->BuildOctTree( );
		m_pChild[ LUB ]->BuildOctTree( );
		m_pChild[ RUF ]->BuildOctTree( );
		m_pChild[ RUB ]->BuildOctTree( );

		//	Down
		m_pChild[ LDF ]->BuildOctTree( );
		m_pChild[ LDB ]->BuildOctTree( );
		m_pChild[ RDF ]->BuildOctTree( );
		m_pChild[ RDB ]->BuildOctTree( );
	}
}

BOOL COctTree2Array::SubDivide( )
{
	if( m_nSize > 1 )
	{
		AREA	ChildArea[ 8 ];

		m_pChild[ LUF ] = AddChild( LUF );
		m_pChild[ LUB ] = AddChild( LUB );
		m_pChild[ RUF ] = AddChild( RUF );
		m_pChild[ RUB ] = AddChild( RUB );

		m_pChild[ LDF ] = AddChild( LDF );
		m_pChild[ LDB ] = AddChild( LDB );
		m_pChild[ RDF ] = AddChild( RDF );
		m_pChild[ RDB ] = AddChild( RDB );

		return TRUE;
	}

	return FALSE;
}

COctTree2Array* COctTree2Array::AddChild( INT nIndex )
{
	COctTree2Array*	pOctTree = new COctTree2Array();
	pOctTree->Cleanup();
	pOctTree->m_nSize = m_nSize >> 1;

	if ( ( m_nSize & 0x01 ) && ( nIndex == RUF || nIndex == RUB || nIndex == RDF || nIndex == RDB) )
	{
		pOctTree->m_nSize += 1;
	}
	
	pOctTree->m_Area.nIndex = -1;
	switch (nIndex)
	{
	case LUF:
		pOctTree->m_Area.vCenter.x = m_Area.vCenter.x - pOctTree->m_nSize;
		pOctTree->m_Area.vCenter.y = m_Area.vCenter.y + pOctTree->m_nSize;
		pOctTree->m_Area.vCenter.z = m_Area.vCenter.z - pOctTree->m_nSize;
		break;
	case LUB:
		pOctTree->m_Area.vCenter.x = m_Area.vCenter.x - pOctTree->m_nSize;
		pOctTree->m_Area.vCenter.y = m_Area.vCenter.y + pOctTree->m_nSize;
		pOctTree->m_Area.vCenter.z = m_Area.vCenter.z + pOctTree->m_nSize;
		break;
	case RUF:
		pOctTree->m_Area.vCenter.x = m_Area.vCenter.x + pOctTree->m_nSize;
		pOctTree->m_Area.vCenter.y = m_Area.vCenter.y + pOctTree->m_nSize;
		pOctTree->m_Area.vCenter.z = m_Area.vCenter.z - pOctTree->m_nSize;
		break;
	case RUB:
		pOctTree->m_Area.vCenter.x = m_Area.vCenter.x + pOctTree->m_nSize;
		pOctTree->m_Area.vCenter.y = m_Area.vCenter.y + pOctTree->m_nSize;
		pOctTree->m_Area.vCenter.z = m_Area.vCenter.z + pOctTree->m_nSize;
		break;
	case LDF:
		pOctTree->m_Area.vCenter.x = m_Area.vCenter.x - pOctTree->m_nSize;
		pOctTree->m_Area.vCenter.y = m_Area.vCenter.y - pOctTree->m_nSize;
		pOctTree->m_Area.vCenter.z = m_Area.vCenter.z - pOctTree->m_nSize;
		break;
	case LDB:
		pOctTree->m_Area.vCenter.x = m_Area.vCenter.x - pOctTree->m_nSize;
		pOctTree->m_Area.vCenter.y = m_Area.vCenter.y - pOctTree->m_nSize;
		pOctTree->m_Area.vCenter.z = m_Area.vCenter.z + pOctTree->m_nSize;
		break;
	case RDF:
		pOctTree->m_Area.vCenter.x = m_Area.vCenter.x + pOctTree->m_nSize;
		pOctTree->m_Area.vCenter.y = m_Area.vCenter.y - pOctTree->m_nSize;
		pOctTree->m_Area.vCenter.z = m_Area.vCenter.z - pOctTree->m_nSize;
		break;
	case RDB:
		pOctTree->m_Area.vCenter.x = m_Area.vCenter.x + pOctTree->m_nSize;
		pOctTree->m_Area.vCenter.y = m_Area.vCenter.y - pOctTree->m_nSize;
		pOctTree->m_Area.vCenter.z = m_Area.vCenter.z + pOctTree->m_nSize;
		break;
 	}

	return pOctTree;
}

VOID COctTree2Array::SetChildArea( LPAREA _pArea )
{
	//	LUB
	//_pArea[ LUB ].nSize =  vMin.x = m_Area.vMin.x;
	//_pArea[ LUB ].vMin.y = m_Area.vMin.y + ( m_Area.vMax.y - m_Area.vMin.y ) / 2.0f;
	//_pArea[ LUB ].vMin.z = m_Area.vMin.z;

	//_pArea[ LUB ].vMax.x = m_Area.vMin.x + ( m_Area.vMax.x - m_Area.vMin.x ) / 2.0f;
	//_pArea[ LUB ].vMax.y = m_Area.vMax.y;
	//_pArea[ LUB ].vMax.z = m_Area.vMin.z + ( m_Area.vMax.z - m_Area.vMin.z ) / 2.0f;

	////	LUF
	//_pArea[ LUF ].vMin.x = m_Area.vMin.x;
	//_pArea[ LUF ].vMin.y = m_Area.vMin.y + ( m_Area.vMax.y - m_Area.vMin.y ) / 2.0f;
	//_pArea[ LUF ].vMin.z = m_Area.vMin.z + ( m_Area.vMax.z - m_Area.vMin.z ) / 2.0f;

	//_pArea[ LUF ].vMax.x = m_Area.vMin.x + ( m_Area.vMax.x - m_Area.vMin.x ) / 2.0f;
	//_pArea[ LUF ].vMax.y = m_Area.vMax.y;
	//_pArea[ LUF ].vMax.z = m_Area.vMax.z;

	////	RUF
	//_pArea[ RUF ].vMin.x = m_Area.vMin.x + ( m_Area.vMax.x - m_Area.vMin.x ) / 2.0f;
	//_pArea[ RUF ].vMin.y = m_Area.vMin.y + ( m_Area.vMax.y - m_Area.vMin.y ) / 2.0f;
	//_pArea[ RUF ].vMin.z = m_Area.vMin.z + ( m_Area.vMax.z - m_Area.vMin.z ) / 2.0f;

	//_pArea[ RUF ].vMax.x = m_Area.vMax.x;
	//_pArea[ RUF ].vMax.y = m_Area.vMax.y;
	//_pArea[ RUF ].vMax.z = m_Area.vMax.z;

	////	RUB
	//_pArea[ RUB ].vMin.x = m_Area.vMin.x + ( m_Area.vMax.x - m_Area.vMin.x ) / 2.0f;
	//_pArea[ RUB ].vMin.y = m_Area.vMin.y + ( m_Area.vMax.y - m_Area.vMin.y ) / 2.0f;
	//_pArea[ RUB ].vMin.z = m_Area.vMin.z;

	//_pArea[ RUB ].vMax.x = m_Area.vMax.x;
	//_pArea[ RUB ].vMax.y = m_Area.vMax.y;
	//_pArea[ RUB ].vMax.z = m_Area.vMin.z + ( m_Area.vMax.z - m_Area.vMin.z ) / 2.0f;

	////	LDF
	//_pArea[ LDF ].vMin.x = m_Area.vMin.x;
	//_pArea[ LDF ].vMin.y = m_Area.vMin.y;
	//_pArea[ LDF ].vMin.z = m_Area.vMin.z + ( m_Area.vMax.z - m_Area.vMin.z ) / 2.0f;

	//_pArea[ LDF ].vMax.x = m_Area.vMin.x + ( m_Area.vMax.x - m_Area.vMin.x ) / 2.0f;
	//_pArea[ LDF ].vMax.y = m_Area.vMin.y + ( m_Area.vMax.y - m_Area.vMin.y ) / 2.0f;
	//_pArea[ LDF ].vMax.z = m_Area.vMax.z;

	////	LDB
	//_pArea[ LDB ].vMin.x = m_Area.vMin.x;
	//_pArea[ LDB ].vMin.y = m_Area.vMin.y;
	//_pArea[ LDB ].vMin.z = m_Area.vMin.z;

	//_pArea[ LDB ].vMax.x = m_Area.vMin.x + ( m_Area.vMax.x - m_Area.vMin.x ) / 2.0f;
	//_pArea[ LDB ].vMax.y = m_Area.vMin.y + ( m_Area.vMax.y - m_Area.vMin.y ) / 2.0f;
	//_pArea[ LDB ].vMax.z = m_Area.vMin.z + ( m_Area.vMax.z - m_Area.vMin.z ) / 2.0f;

	////	RDF
	//_pArea[ RDF ].vMin.x = m_Area.vMin.x + ( m_Area.vMax.x - m_Area.vMin.x ) / 2.0f;
	//_pArea[ RDF ].vMin.y = m_Area.vMin.y;
	//_pArea[ RDF ].vMin.z = m_Area.vMin.z + ( m_Area.vMax.z - m_Area.vMin.z ) / 2.0f;

	//_pArea[ RDF ].vMax.x = m_Area.vMax.x;
	//_pArea[ RDF ].vMax.y = m_Area.vMin.y + ( m_Area.vMax.y - m_Area.vMin.y ) / 2.0f;
	//_pArea[ RDF ].vMax.z = m_Area.vMax.z;

	////	RDB
	//_pArea[ RDB ].vMin.x = m_Area.vMin.x + ( m_Area.vMax.x - m_Area.vMin.x ) / 2.0f;
	//_pArea[ RDB ].vMin.y = m_Area.vMin.y;
	//_pArea[ RDB ].vMin.z = m_Area.vMin.z;

	//_pArea[ RDB ].vMax.x = m_Area.vMax.x;
	//_pArea[ RDB ].vMax.y = m_Area.vMin.y + ( m_Area.vMax.y - m_Area.vMin.y ) / 2.0f;
	//_pArea[ RDB ].vMax.z = m_Area.vMin.z + ( m_Area.vMax.z - m_Area.vMin.z ) / 2.0f;

}

VOID COctTree2Array::Traverse( D3DXVECTOR3 _Vertex )
{
//	//if( m_pChild[ 0 ] == NULL )
//	//{
//	//	m_Area.bExist = TRUE;
//	//	return;
//	//}
//	//if( CmpMinMax( m_pChild[ LUB ]->m_Area.vMin, m_pChild[ LUB ]->m_Area.vMax, _Vertex ) )
//	//	m_pChild[ LUB ]->Traverse( _Vertex );
//
//	//if( CmpMinMax( m_pChild[ LUF ]->m_Area.vMin, m_pChild[ LUF ]->m_Area.vMax, _Vertex ) )
//	//	m_pChild[ LUF ]->Traverse( _Vertex );
//}
//
//<<<<<<< .mine
//	//if( CmpMinMax( m_pChild[ RUB ]->m_Area.vMin, m_pChild[ RUB ]->m_Area.vMax, _Vertex ) )
//	//	m_pChild[ RUB ]->Traverse( _Vertex );
//=======
//VOID OctTree::GenerateIndex( D3DXVECTOR3& _pvVertex )
//{
//	Traverse( _pvVertex );
//}
//>>>>>>> .r74
//
//<<<<<<< .mine
//	//if( CmpMinMax( m_pChild[ RUF ]->m_Area.vMin, m_pChild[ RUF ]->m_Area.vMax, _Vertex ) )
//	//	m_pChild[ RUF ]->Traverse( _Vertex );
//=======
//
//VOID OctTree::Traverse( D3DXVECTOR3 _Vertex )
//{
//	if( m_pChild[ 0 ] == NULL )
//	{
//		m_Area.bExist = TRUE;
//		return;
//	}
//	if( CmpMinMax( m_pChild[ LUB ]->m_Area.vMin, m_pChild[ LUB ]->m_Area.vMax, _Vertex ) )
//		m_pChild[ LUB ]->Traverse( _Vertex );
//>>>>>>> .r74
//
//	//if( CmpMinMax( m_pChild[ LDB ]->m_Area.vMin, m_pChild[ LDB ]->m_Area.vMax, _Vertex ) )
//	//	m_pChild[ LDB ]->Traverse( _Vertex );
//
//	//if( CmpMinMax( m_pChild[ LDF ]->m_Area.vMin, m_pChild[ LDF ]->m_Area.vMax, _Vertex ) )
//	//	m_pChild[ LDF ]->Traverse( _Vertex );
//
//	//if( CmpMinMax( m_pChild[ RDB ]->m_Area.vMin, m_pChild[ RDB ]->m_Area.vMax, _Vertex ) )
//	//	m_pChild[ RDB ]->Traverse( _Vertex );
//
//	//if( CmpMinMax( m_pChild[ RDF ]->m_Area.vMin, m_pChild[ RDF ]->m_Area.vMax, _Vertex ) )
//	//	m_pChild[ RDF ]->Traverse( _Vertex );
//	//
//	
}


BOOL COctTree2Array::CmpMinMax( const D3DXVECTOR3& _vMin, const D3DXVECTOR3& _vMax, const D3DXVECTOR3& _vVertex )
{
	BOOL bResult  = TRUE;

	if( _vMin.x > _vVertex.x && _vMax.x < _vVertex.x )
		bResult = FALSE;
	if( _vMin.y > _vVertex.y && _vMax.y < _vVertex.y )
		bResult = FALSE;
	if( _vMin.z > _vVertex.z && _vMax.z < _vVertex.z )
		bResult = FALSE;

	return bResult;
}

VOID COctTree2Array::SetChildIndex( D3DXVECTOR3& _pvVertex, INT nIndex)
{
	if ( _pvVertex == m_Area.vCenter )
	{
		m_Area.nIndex = nIndex;

		return;
	}
	if (m_nSize == 1)
		return;

	INT x, y, z;
	if (_pvVertex.x >= m_Area.vCenter.x)	x = 1;
	else									x = 0;
	if (_pvVertex.y >= m_Area.vCenter.y)	y = 1;
	else									y = 0;
	if (_pvVertex.z >= m_Area.vCenter.z)	z = 1;
	else									z = 0;

	if      ( x == 0 && y == 1 && z == 0 )	m_pChild[LUF]->SetChildIndex( _pvVertex, nIndex );
	else if ( x == 0 && y == 1 && z == 1 )	m_pChild[LUB]->SetChildIndex( _pvVertex, nIndex );
	else if ( x == 1 && y == 1 && z == 0 )	m_pChild[RUF]->SetChildIndex( _pvVertex, nIndex );
	else if ( x == 1 && y == 1 && z == 1 )	m_pChild[RUB]->SetChildIndex( _pvVertex, nIndex );
	else if ( x == 0 && y == 0 && z == 0 )	m_pChild[LDF]->SetChildIndex( _pvVertex, nIndex );
	else if ( x == 0 && y == 0 && z == 1 )	m_pChild[LDB]->SetChildIndex( _pvVertex, nIndex );
	else if ( x == 1 && y == 0 && z == 0 )	m_pChild[RDF]->SetChildIndex( _pvVertex, nIndex );
	else if ( x == 1 && y == 0 && z == 1 )	m_pChild[RDB]->SetChildIndex( _pvVertex, nIndex );
}

INT COctTree2Array::GetChildIndex( D3DXVECTOR3& _pvVertex )
{
	if ( _pvVertex == m_Area.vCenter )
	{
		return  m_Area.nIndex;
	}

	INT x, y, z;
	if (_pvVertex.x >= m_Area.vCenter.x)	x = 1;
	else									x = 0;
	if (_pvVertex.y >= m_Area.vCenter.y)	y = 1;
	else									y = 0;
	if (_pvVertex.z >= m_Area.vCenter.z)	z = 1;
	else									z = 0;

	if      ( x == 0 && y == 1 && z == 0 )	m_pChild[LUF]->GetChildIndex( _pvVertex );
	else if ( x == 0 && y == 1 && z == 1 )	m_pChild[LUB]->GetChildIndex( _pvVertex );
	else if ( x == 1 && y == 1 && z == 0 )	m_pChild[RUF]->GetChildIndex( _pvVertex );
	else if ( x == 1 && y == 1 && z == 1 )	m_pChild[RUB]->GetChildIndex( _pvVertex );
	else if ( x == 0 && y == 0 && z == 0 )	m_pChild[LDF]->GetChildIndex( _pvVertex );
	else if ( x == 0 && y == 0 && z == 1 )	m_pChild[LDB]->GetChildIndex( _pvVertex );
	else if ( x == 1 && y == 0 && z == 0 )	m_pChild[RDF]->GetChildIndex( _pvVertex );
	else if ( x == 1 && y == 0 && z == 1 )	m_pChild[RDB]->GetChildIndex( _pvVertex );

	return -1;
}
