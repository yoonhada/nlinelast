#include "stdafx.h"
#include <assert.h>

CTree::CTree()
: m_pRoot( NULL )
, m_pCurr( NULL ) 
{
}

CTree::CTree( NODE* _pRoot ) 
: m_pRoot ( _pRoot ) 
, m_pCurr( NULL ) 
{
}

CTree::~CTree()
{
	m_pCurr = m_pRoot; 
	Release(); 
} 

HRESULT CTree::Create( FLOAT fRange, INT nDeep )
{
	if (m_pRoot)
		Release();

	m_pRoot = new NODE;
	CreateChild( m_pRoot, -fRange, fRange, fRange, -fRange, nDeep );


	return S_OK; 
}

VOID CTree::Release()			
{
	if( m_pCurr != NULL )
	{
		if ( m_pCurr->_children[NODE::LT] == NULL )
		{
			SAFE_DELETE(m_pCurr);
		}
		else
		{
			NODE * pParent = m_pCurr;
			if ( pParent->_children[NODE::LT] != NULL )
			{
				m_pCurr = pParent->_children[NODE::LT];
				Release();
			}
			if ( pParent->_children[NODE::RT] != NULL )
			{
				m_pCurr = pParent->_children[NODE::RT];
				Release();
			}
			if ( pParent->_children[NODE::LB] != NULL )
			{
				m_pCurr = pParent->_children[NODE::LB];
				Release();
			}
			if ( pParent->_children[NODE::RB] != NULL )
			{
				m_pCurr = pParent->_children[NODE::RB];
				Release();
			}

			SAFE_DELETE(pParent);
		}
	}
}


VOID CTree::InsertObject( NODE * pNode, CBoundBox * pCube )
{	
	assert(pNode != NULL);

	if ( IsRect( pNode, pCube ) )
	{
		if ( pNode->_children[NODE::LT] == NULL )
		{
			pNode->_data.push_back( pCube );
			return;
		}

		if ( IsRect( pNode->_children[NODE::LT], pCube ) )
		{
			InsertObject( pNode->_children[NODE::LT], pCube );
		}
		if ( IsRect( pNode->_children[NODE::RT], pCube ) )
		{
			InsertObject( pNode->_children[NODE::RT], pCube );
		}
		if ( IsRect( pNode->_children[NODE::LB], pCube ) )
		{
			InsertObject( pNode->_children[NODE::LB], pCube );
		}
		if ( IsRect( pNode->_children[NODE::RB], pCube ) )
		{
			InsertObject( pNode->_children[NODE::RB], pCube );
		}
	}
}

VOID CTree::RemoveObject( NODE * pNode, CBoundBox * pCube )
{
	assert(pNode != NULL);

	if ( IsRect( pNode, pCube ) )
	{
		if ( pNode->_children[NODE::LT] == NULL )
		{
			std::vector <CBoundBox*>::iterator Iter = find(pNode->_data.begin(), pNode->_data.end(), pCube);
			pNode->_data.erase(Iter);
			return;
		}

		if ( IsRect( pNode->_children[NODE::LT], pCube ) )
			RemoveObject( pNode->_children[NODE::LT], pCube );
		if ( IsRect( pNode->_children[NODE::RT], pCube ) )
			RemoveObject( pNode->_children[NODE::RT], pCube );
		if ( IsRect( pNode->_children[NODE::LB], pCube ) )
			RemoveObject( pNode->_children[NODE::LB], pCube );
		if ( IsRect( pNode->_children[NODE::RB], pCube ) )
			RemoveObject( pNode->_children[NODE::RB], pCube );
	}
}

VOID CTree::RemoveAll( )
{
	if ( m_pCurr->_children[NODE::LT] == NULL )
	{
		m_pCurr->_data.erase( m_pCurr->_data.begin(), m_pCurr->_data.end() );
	}
	else
	{
		NODE * pParent = m_pCurr;
		if ( pParent->_children[NODE::LT] != NULL )
		{
			m_pCurr = pParent->_children[NODE::LT];
			RemoveAll();
		}
		if ( pParent->_children[NODE::RT] != NULL )
		{
			m_pCurr = pParent->_children[NODE::RT];
			RemoveAll();
		}
		if ( pParent->_children[NODE::LB] != NULL )
		{
			m_pCurr = pParent->_children[NODE::LB];
			RemoveAll();
		}
		if ( pParent->_children[NODE::RB] != NULL )
		{
			m_pCurr = pParent->_children[NODE::RB];
			RemoveAll();
		}
	}
}

VOID CTree::CreateChild( NODE *pNode, FLOAT fLeft, FLOAT fTop, FLOAT fRight, FLOAT fBottom, INT nDeep )
{
	assert(pNode != NULL);
	pNode->_pos[NODE::LT] = D3DXVECTOR3(  fLeft, 0,    fTop);
	pNode->_pos[NODE::RT] = D3DXVECTOR3( fRight, 0,    fTop);
	pNode->_pos[NODE::LB] = D3DXVECTOR3(  fLeft, 0, fBottom);
	pNode->_pos[NODE::RB] = D3DXVECTOR3( fRight, 0, fBottom);

	if ( nDeep > 0 )
	{
		nDeep--;
		FLOAT fCen = ( fLeft + fRight ) * 0.5f;
		FLOAT fMid = ( fTop + fBottom ) * 0.5f;

		pNode->_children[NODE::LT] = new NODE();
		CreateChild( pNode->_children[NODE::LT], fLeft, fTop, fCen, fMid, nDeep );
		
		pNode->_children[NODE::RT] = new NODE();
		CreateChild( pNode->_children[NODE::RT], fCen, fTop, fRight, fMid, nDeep );
		
		pNode->_children[NODE::LB] = new NODE();
		CreateChild( pNode->_children[NODE::LB], fLeft, fMid, fCen, fBottom, nDeep );
		
		pNode->_children[NODE::RB] = new NODE();
		CreateChild( pNode->_children[NODE::RB], fCen, fMid, fRight, fBottom, nDeep );
	}
}

BOOL CTree::IsRect( NODE *pNode, CBoundBox * pBoundBox)
{
	assert(pNode != NULL);

	FRECT rect2 = pBoundBox->GetFRect();
	FRECT rect1 = pNode->GetRect();

	if ( rect1.left > rect2.right )
		return FALSE;
	if ( rect1.right < rect2.left )
		return FALSE;
	if ( rect1.top < rect2.bottom )
		return FALSE;
	if ( rect1.bottom > rect2.top )
		return FALSE;

	return TRUE;
}

BOOL CTree::IsRect( NODE *pNode, D3DXVECTOR3 &vPos)
{
	assert(pNode != NULL);
	BOOL bRet = FALSE;
	FRECT rect = pNode->GetRect();

	if ( rect.left <= vPos.x && vPos.x <= rect.right &&
		 rect.bottom <= vPos.z && vPos.z <= rect.top )
		 bRet = TRUE;

	return bRet;
}

std::vector <CBoundBox*> * CTree::GetMapVector( NODE *pNode, D3DXVECTOR3 vPos )
{
	assert(pNode != NULL);

	if ( pNode->_children[NODE::LT] == NULL )
	{
		return &pNode->_data;
	}

	if ( IsRect( pNode->_children[NODE::LT], vPos ) )	
		return GetMapVector( pNode->_children[NODE::LT], vPos );
	if ( IsRect( pNode->_children[NODE::RT], vPos ) )		
		return GetMapVector( pNode->_children[NODE::RT], vPos );
	if ( IsRect( pNode->_children[NODE::LB], vPos ) )		
		return GetMapVector( pNode->_children[NODE::LB], vPos );
	if ( IsRect( pNode->_children[NODE::RB], vPos ) )		
		return GetMapVector( pNode->_children[NODE::RB], vPos );

	return NULL;
}
