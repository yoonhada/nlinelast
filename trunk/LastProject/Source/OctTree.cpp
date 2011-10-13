#include "stdafx.h"
#include "OctTree.h"

VOID OctTree::Initialize()
{
	for( INT i=0 ; i<8 ; i++ )
		m_pChild[ i ] = NULL;

	m_iLevel = 0;
}

VOID OctTree::Release()
{
	Cleanup();
}

VOID OctTree::Cleanup()
{
	for( INT i=0 ; i<8 ; i++ )
	{
		if( m_pChild[ i ] != NULL )
		{
			delete m_pChild[ i ];
			m_pChild[ i ] = NULL;
		}
	}
}

VOID OctTree::Build( INT _iLevel, D3DXVECTOR3 _vMin, D3DXVECTOR3 _vMax )
{
	m_Area.vMin = _vMin;
	m_Area.vMax = _vMax;

	BuildOctTree( _iLevel );
}

VOID OctTree::Build( INT _iLevel, LPD3DXVECTOR3 _pvVertex, INT _iSize )
{
	//	CleanupÀ¸·Î ¹Ù²Ü·¡
	Cleanup();

	GetMinMax( _pvVertex, _iSize );

	BuildOctTree( _iLevel );
}
VOID OctTree::GetMinMax( LPD3DXVECTOR3 _pvVertex, INT _iSize )
{
	m_Area.vMin = _pvVertex[ 0 ];
	m_Area.vMax = _pvVertex[ 0 ];

	for( INT i=0 ; i<_iSize ; i++ )
	{
		if( m_Area.vMin.x > _pvVertex[ i ].x )
			m_Area.vMin.x = _pvVertex[ i ].x;
		if( m_Area.vMin.y > _pvVertex[ i ].y )
			m_Area.vMin.y = _pvVertex[ i ].y;
		if( m_Area.vMin.z > _pvVertex[ i ].z )
			m_Area.vMin.z = _pvVertex[ i ].z;

		if( m_Area.vMax.x < _pvVertex[ i ].x )
			m_Area.vMax.x = _pvVertex[ i ].x;
		if( m_Area.vMax.y < _pvVertex[ i ].y )
			m_Area.vMax.y = _pvVertex[ i ].y;
		if( m_Area.vMax.z < _pvVertex[ i ].z )
			m_Area.vMax.z = _pvVertex[ i ].z;
	}
}

VOID OctTree::BuildOctTree( INT _iLevel )
{
	m_iLevel = _iLevel;

	if( SubDivide( _iLevel ) )
	{
		//	Up
		m_pChild[ LUF ]->BuildOctTree( _iLevel - 1 );
		m_pChild[ LUB ]->BuildOctTree( _iLevel - 1 );
		m_pChild[ RUF ]->BuildOctTree( _iLevel - 1 );
		m_pChild[ RUB ]->BuildOctTree( _iLevel - 1 );

		//	Down
		m_pChild[ LDF ]->BuildOctTree( _iLevel - 1 );
		m_pChild[ LDB ]->BuildOctTree( _iLevel - 1 );
		m_pChild[ RDF ]->BuildOctTree( _iLevel - 1 );
		m_pChild[ RDB ]->BuildOctTree( _iLevel - 1 );

	}
}

BOOL OctTree::SubDivide( INT _iLevel )
{
	if( IsDivide() && _iLevel > 0 )
	{
		AREA	ChildArea[ 8 ];
		SetChildArea( ChildArea );

		/*CDebugConsole::GetInstance()->Messagef( L"[ Parent ] Min x : %f y : %f z : %f\n", m_Area.vMin.x, m_Area.vMin.y, m_Area.vMin.z );
		CDebugConsole::GetInstance()->Messagef( L"[ Parent ] Max x : %f y : %f z : %f\n", m_Area.vMax.x, m_Area.vMax.y, m_Area.vMax.z );*/
		
		//	Up
		m_pChild[ LUF ] = AddChild( ChildArea[ LUF ].vMin, ChildArea[ LUF ].vMax );
		m_pChild[ LUB ] = AddChild( ChildArea[ LUB ].vMin, ChildArea[ LUB ].vMax );
		m_pChild[ RUF ] = AddChild( ChildArea[ RUF ].vMin, ChildArea[ RUF ].vMax );
		m_pChild[ RUB ] = AddChild( ChildArea[ RUB ].vMin, ChildArea[ RUB ].vMax );

		/*CDebugConsole::GetInstance()->Messagef( L"[ LUF ] Min x : %f y : %f z : %f\n", ChildArea[ LUF ].vMin.x, ChildArea[ LUF ].vMin.y, ChildArea[ LUF ].vMin.z );
		CDebugConsole::GetInstance()->Messagef( L"[ LUF ] Max x : %f y : %f z : %f\n", ChildArea[ LUF ].vMax.x, ChildArea[ LUF ].vMax.y, ChildArea[ LUF ].vMax.z );

		CDebugConsole::GetInstance()->Messagef( L"[ LUB ] Min x : %f y : %f z : %f\n", ChildArea[ LUB ].vMin.x, ChildArea[ LUB ].vMin.y, ChildArea[ LUB ].vMin.z );
		CDebugConsole::GetInstance()->Messagef( L"[ LUB ] Max x : %f y : %f z : %f\n", ChildArea[ LUB ].vMax.x, ChildArea[ LUB ].vMax.y, ChildArea[ LUB ].vMax.z );

		CDebugConsole::GetInstance()->Messagef( L"[ RUF ] Min x : %f y : %f z : %f\n", ChildArea[ RUF ].vMin.x, ChildArea[ RUF ].vMin.y, ChildArea[ RUF ].vMin.z );
		CDebugConsole::GetInstance()->Messagef( L"[ RUF ] Max x : %f y : %f z : %f\n", ChildArea[ RUF ].vMax.x, ChildArea[ RUF ].vMax.y, ChildArea[ RUF ].vMax.z );

		CDebugConsole::GetInstance()->Messagef( L"[ RUB ] Min x : %f y : %f z : %f\n", ChildArea[ RUB ].vMin.x, ChildArea[ RUB ].vMin.y, ChildArea[ RUB ].vMin.z );
		CDebugConsole::GetInstance()->Messagef( L"[ RUB ] Max x : %f y : %f z : %f\n", ChildArea[ RUB ].vMax.x, ChildArea[ RUB ].vMax.y, ChildArea[ RUB ].vMax.z );
		*/

		//	Down
		m_pChild[ LDF ] = AddChild( ChildArea[ LDF ].vMin, ChildArea[ LDF ].vMax );
		m_pChild[ LDB ] = AddChild( ChildArea[ LDB ].vMin, ChildArea[ LDB ].vMax );
		m_pChild[ RDF ] = AddChild( ChildArea[ RDF ].vMin, ChildArea[ RDF ].vMax );
		m_pChild[ RDB ] = AddChild( ChildArea[ RDB ].vMin, ChildArea[ RDB ].vMax );

		/*CDebugConsole::GetInstance()->Messagef( L"[ LDF ] Min x : %f y : %f z : %f\n", ChildArea[ LDF ].vMin.x, ChildArea[ LDF ].vMin.y, ChildArea[ LDF ].vMin.z );
		CDebugConsole::GetInstance()->Messagef( L"[ LDF ] Max x : %f y : %f z : %f\n", ChildArea[ LDF ].vMax.x, ChildArea[ LDF ].vMax.y, ChildArea[ LDF ].vMax.z );

		CDebugConsole::GetInstance()->Messagef( L"[ LDB ] Min x : %f y : %f z : %f\n", ChildArea[ LDB ].vMin.x, ChildArea[ LDB ].vMin.y, ChildArea[ LDB ].vMin.z );
		CDebugConsole::GetInstance()->Messagef( L"[ LDB ] Max x : %f y : %f z : %f\n", ChildArea[ LDB ].vMax.x, ChildArea[ LDB ].vMax.y, ChildArea[ LDB ].vMax.z );

		CDebugConsole::GetInstance()->Messagef( L"[ RDF ] Min x : %f y : %f z : %f\n", ChildArea[ RDF ].vMin.x, ChildArea[ RDF ].vMin.y, ChildArea[ RDF ].vMin.z );
		CDebugConsole::GetInstance()->Messagef( L"[ RDF ] Max x : %f y : %f z : %f\n", ChildArea[ RDF ].vMax.x, ChildArea[ RDF ].vMax.y, ChildArea[ RDF ].vMax.z );

		CDebugConsole::GetInstance()->Messagef( L"[ RDB ] Min x : %f y : %f z : %f\n", ChildArea[ RDB ].vMin.x, ChildArea[ RDB ].vMin.y, ChildArea[ RDB ].vMin.z );
		CDebugConsole::GetInstance()->Messagef( L"[ RDB ] Max x : %f y : %f z : %f\n", ChildArea[ RDB ].vMax.x, ChildArea[ RDB ].vMax.y, ChildArea[ RDB ].vMax.z );*/

		return TRUE;
	}

	return FALSE;
}

BOOL OctTree::IsDivide()
{
	BOOL	bPosible = TRUE;

	if( m_Area.vMax.x - m_Area.vMin.x < 2.0f )
		bPosible = FALSE;
	if( m_Area.vMax.y - m_Area.vMin.y < 2.0f )
		bPosible = FALSE;
	if( m_Area.vMax.z - m_Area.vMin.z < 2.0f )
		bPosible = FALSE;

	return bPosible;
}

OctTree* OctTree::AddChild( D3DXVECTOR3 _vMin, D3DXVECTOR3 _vMax )
{
	OctTree*	pOctTree = new OctTree();

	pOctTree->m_Area.vMin = _vMin;
	pOctTree->m_Area.vMax = _vMax;
	
	pOctTree->Cleanup();

	return pOctTree;
}

VOID OctTree::SetChildArea( LPAREA _pArea )
{
	//	LUB
	_pArea[ LUB ].vMin.x = m_Area.vMin.x;
	_pArea[ LUB ].vMin.y = m_Area.vMin.y + ( m_Area.vMax.y - m_Area.vMin.y ) / 2.0f;
	_pArea[ LUB ].vMin.z = m_Area.vMin.z;

	_pArea[ LUB ].vMax.x = m_Area.vMin.x + ( m_Area.vMax.x - m_Area.vMin.x ) / 2.0f;
	_pArea[ LUB ].vMax.y = m_Area.vMax.y;
	_pArea[ LUB ].vMax.z = m_Area.vMin.z + ( m_Area.vMax.z - m_Area.vMin.z ) / 2.0f;

	//	LUF
	_pArea[ LUF ].vMin.x = m_Area.vMin.x;
	_pArea[ LUF ].vMin.y = m_Area.vMin.y + ( m_Area.vMax.y - m_Area.vMin.y ) / 2.0f;
	_pArea[ LUF ].vMin.z = m_Area.vMin.z + ( m_Area.vMax.z - m_Area.vMin.z ) / 2.0f;

	_pArea[ LUF ].vMax.x = m_Area.vMin.x + ( m_Area.vMax.x - m_Area.vMin.x ) / 2.0f;
	_pArea[ LUF ].vMax.y = m_Area.vMax.y;
	_pArea[ LUF ].vMax.z = m_Area.vMax.z;

	//	RUF
	_pArea[ RUF ].vMin.x = m_Area.vMin.x + ( m_Area.vMax.x - m_Area.vMin.x ) / 2.0f;
	_pArea[ RUF ].vMin.y = m_Area.vMin.y + ( m_Area.vMax.y - m_Area.vMin.y ) / 2.0f;
	_pArea[ RUF ].vMin.z = m_Area.vMin.z + ( m_Area.vMax.z - m_Area.vMin.z ) / 2.0f;

	_pArea[ RUF ].vMax.x = m_Area.vMax.x;
	_pArea[ RUF ].vMax.y = m_Area.vMax.y;
	_pArea[ RUF ].vMax.z = m_Area.vMax.z;

	//	RUB
	_pArea[ RUB ].vMin.x = m_Area.vMin.x + ( m_Area.vMax.x - m_Area.vMin.x ) / 2.0f;
	_pArea[ RUB ].vMin.y = m_Area.vMin.y + ( m_Area.vMax.y - m_Area.vMin.y ) / 2.0f;
	_pArea[ RUB ].vMin.z = m_Area.vMin.z;

	_pArea[ RUB ].vMax.x = m_Area.vMax.x;
	_pArea[ RUB ].vMax.y = m_Area.vMax.y;
	_pArea[ RUB ].vMax.z = m_Area.vMin.z + ( m_Area.vMax.z - m_Area.vMin.z ) / 2.0f;

	//	LDF
	_pArea[ LDF ].vMin.x = m_Area.vMin.x;
	_pArea[ LDF ].vMin.y = m_Area.vMin.y;
	_pArea[ LDF ].vMin.z = m_Area.vMin.z + ( m_Area.vMax.z - m_Area.vMin.z ) / 2.0f;

	_pArea[ LDF ].vMax.x = m_Area.vMin.x + ( m_Area.vMax.x - m_Area.vMin.x ) / 2.0f;
	_pArea[ LDF ].vMax.y = m_Area.vMin.y + ( m_Area.vMax.y - m_Area.vMin.y ) / 2.0f;
	_pArea[ LDF ].vMax.z = m_Area.vMax.z;

	//	LDB
	_pArea[ LDB ].vMin.x = m_Area.vMin.x;
	_pArea[ LDB ].vMin.y = m_Area.vMin.y;
	_pArea[ LDB ].vMin.z = m_Area.vMin.z;

	_pArea[ LDB ].vMax.x = m_Area.vMin.x + ( m_Area.vMax.x - m_Area.vMin.x ) / 2.0f;
	_pArea[ LDB ].vMax.y = m_Area.vMin.y + ( m_Area.vMax.y - m_Area.vMin.y ) / 2.0f;
	_pArea[ LDB ].vMax.z = m_Area.vMin.z + ( m_Area.vMax.z - m_Area.vMin.z ) / 2.0f;

	//	RDF
	_pArea[ RDF ].vMin.x = m_Area.vMin.x + ( m_Area.vMax.x - m_Area.vMin.x ) / 2.0f;
	_pArea[ RDF ].vMin.y = m_Area.vMin.y;
	_pArea[ RDF ].vMin.z = m_Area.vMin.z + ( m_Area.vMax.z - m_Area.vMin.z ) / 2.0f;

	_pArea[ RDF ].vMax.x = m_Area.vMax.x;
	_pArea[ RDF ].vMax.y = m_Area.vMin.y + ( m_Area.vMax.y - m_Area.vMin.y ) / 2.0f;
	_pArea[ RDF ].vMax.z = m_Area.vMax.z;

	//	RDB
	_pArea[ RDB ].vMin.x = m_Area.vMin.x + ( m_Area.vMax.x - m_Area.vMin.x ) / 2.0f;
	_pArea[ RDB ].vMin.y = m_Area.vMin.y;
	_pArea[ RDB ].vMin.z = m_Area.vMin.z;

	_pArea[ RDB ].vMax.x = m_Area.vMax.x;
	_pArea[ RDB ].vMax.y = m_Area.vMin.y + ( m_Area.vMax.y - m_Area.vMin.y ) / 2.0f;
	_pArea[ RDB ].vMax.z = m_Area.vMin.z + ( m_Area.vMax.z - m_Area.vMin.z ) / 2.0f;

}

VOID OctTree::GenerateBoundBox( OctTree::LPAREALIST& _pAreaList, LPD3DXVECTOR3 _pvVertex, INT _iSize )
{
	for( INT i=0 ; i<_iSize ; i++ )
		Traverse( _pvVertex[ i ] );

	GetBoundBox( _pAreaList );

}

VOID OctTree::Traverse( D3DXVECTOR3 _Vertex )
{
	if( m_pChild[ 0 ] == NULL )
	{
		m_Area.bExist = TRUE;
		return;
	}
	if( CmpMinMax( m_pChild[ LUB ]->m_Area.vMin, m_pChild[ LUB ]->m_Area.vMax, _Vertex ) )
		m_pChild[ LUB ]->Traverse( _Vertex );

	if( CmpMinMax( m_pChild[ LUF ]->m_Area.vMin, m_pChild[ LUF ]->m_Area.vMax, _Vertex ) )
		m_pChild[ LUF ]->Traverse( _Vertex );

	if( CmpMinMax( m_pChild[ RUB ]->m_Area.vMin, m_pChild[ RUB ]->m_Area.vMax, _Vertex ) )
		m_pChild[ RUB ]->Traverse( _Vertex );

	if( CmpMinMax( m_pChild[ RUF ]->m_Area.vMin, m_pChild[ RUF ]->m_Area.vMax, _Vertex ) )
		m_pChild[ RUF ]->Traverse( _Vertex );

	if( CmpMinMax( m_pChild[ LDB ]->m_Area.vMin, m_pChild[ LDB ]->m_Area.vMax, _Vertex ) )
		m_pChild[ LDB ]->Traverse( _Vertex );

	if( CmpMinMax( m_pChild[ LDF ]->m_Area.vMin, m_pChild[ LDF ]->m_Area.vMax, _Vertex ) )
		m_pChild[ LDF ]->Traverse( _Vertex );

	if( CmpMinMax( m_pChild[ RDB ]->m_Area.vMin, m_pChild[ RDB ]->m_Area.vMax, _Vertex ) )
		m_pChild[ RDB ]->Traverse( _Vertex );

	if( CmpMinMax( m_pChild[ RDF ]->m_Area.vMin, m_pChild[ RDF ]->m_Area.vMax, _Vertex ) )
		m_pChild[ RDF ]->Traverse( _Vertex );
	
	
}

VOID OctTree::GetBoundBox( OctTree::LPAREALIST& _pArealist )
{
	if( m_pChild[ 0 ] != NULL )
	{
		m_pChild[ LUF ]->GetBoundBox( _pArealist );
		m_pChild[ LUB ]->GetBoundBox( _pArealist );
		
		m_pChild[ RUB ]->GetBoundBox( _pArealist );
		m_pChild[ RUF ]->GetBoundBox( _pArealist );

		m_pChild[ LDB ]->GetBoundBox( _pArealist );
		m_pChild[ LDF ]->GetBoundBox( _pArealist );

		m_pChild[ RDB ]->GetBoundBox( _pArealist );
		m_pChild[ RDF ]->GetBoundBox( _pArealist );
	}
	else
	{
		if( m_Area.bExist == FALSE )
			return;
		LPAREA	pArea = new AREA;
		pArea->vMin = m_Area.vMin;
		pArea->vMax = m_Area.vMax;

		_pArealist.push_back( pArea );
	}

}

BOOL OctTree::CmpMinMax( const D3DXVECTOR3& _vMin, const D3DXVECTOR3& _vMax, const D3DXVECTOR3& _vVertex )
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