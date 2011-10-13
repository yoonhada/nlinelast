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

	// SAFE_DELETE(m_pRoot);
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
	if ( m_pCurr->_children[NODE::TL] == NULL )
	{
		m_pCurr->_data.erase(m_pCurr->_data.begin(), m_pCurr->_data.end());
		SAFE_DELETE(m_pCurr);
	}
	else
	{
		NODE * pParent = m_pCurr;
		if ( pParent->_children[NODE::TL] != NULL )
		{
			m_pCurr = pParent->_children[NODE::TL];
			Release();
		}
		if ( pParent->_children[NODE::TR] != NULL )
		{
			m_pCurr = pParent->_children[NODE::TR];
			Release();
		}
		if ( pParent->_children[NODE::BL] != NULL )
		{
			m_pCurr = pParent->_children[NODE::BL];
			Release();
		}
		if ( pParent->_children[NODE::BR] != NULL )
		{
			m_pCurr = pParent->_children[NODE::BR];
			Release();
		}

		SAFE_DELETE(pParent);
	}
}


VOID CTree::InsertObject( NODE * pNode, CBoundBox * pCube )
{	
	assert(pNode != NULL);

	if ( IsRect( pNode, pCube ) )
	{
		if ( pNode->_children[NODE::TL] == NULL )
		{
			pNode->_data.push_back( pCube );
			return;
		}

		if ( IsRect( pNode->_children[NODE::TL], pCube ) )
			InsertObject( pNode->_children[NODE::TL], pCube );
		if ( IsRect( pNode->_children[NODE::TR], pCube ) )
			InsertObject( pNode->_children[NODE::TR], pCube );
		if ( IsRect( pNode->_children[NODE::BL], pCube ) )
			InsertObject( pNode->_children[NODE::BL], pCube );
		if ( IsRect( pNode->_children[NODE::BR], pCube ) )
			InsertObject( pNode->_children[NODE::BR], pCube );
	}
}

VOID CTree::RemoveObject( NODE * pNode, CBoundBox * pCube )
{
	assert(pNode != NULL);

	if ( IsRect( pNode, pCube ) )
	{
		if ( pNode->_children[NODE::TL] == NULL )
		{
			std::vector <CBoundBox*>::iterator Iter = find(pNode->_data.begin(), pNode->_data.end(), pCube);
			pNode->_data.erase(Iter);
			return;
		}

		if ( IsRect( pNode->_children[NODE::TL], pCube ) )
			RemoveObject( pNode->_children[NODE::TL], pCube );
		if ( IsRect( pNode->_children[NODE::TR], pCube ) )
			RemoveObject( pNode->_children[NODE::TR], pCube );
		if ( IsRect( pNode->_children[NODE::BL], pCube ) )
			RemoveObject( pNode->_children[NODE::BL], pCube );
		if ( IsRect( pNode->_children[NODE::BR], pCube ) )
			RemoveObject( pNode->_children[NODE::BR], pCube );
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/

VOID CTree::CreateChild( NODE *pNode, FLOAT fLeft, FLOAT fTop, FLOAT fRight, FLOAT fBottom, INT nDeep )
{
	assert(pNode != NULL);
	pNode->_pos[NODE::TL] = D3DXVECTOR3(  fLeft, 0,    fTop);
	pNode->_pos[NODE::TR] = D3DXVECTOR3( fRight, 0,    fTop);
	pNode->_pos[NODE::BL] = D3DXVECTOR3(  fLeft, 0, fBottom);
	pNode->_pos[NODE::BR] = D3DXVECTOR3( fRight, 0, fBottom);

	if ( nDeep > 0 )
	{
		nDeep--;
		FLOAT fCen = ( fLeft + fRight ) * 0.5f;
		FLOAT fMid = ( fTop + fBottom ) * 0.5f;

		pNode->_children[NODE::TL] = new NODE();
		CreateChild( pNode->_children[NODE::TL], fLeft, fTop, fCen, fMid, nDeep );
		
		pNode->_children[NODE::TR] = new NODE();
		CreateChild( pNode->_children[NODE::TR], fCen, fTop, fRight, fMid, nDeep );
		
		pNode->_children[NODE::BL] = new NODE();
		CreateChild( pNode->_children[NODE::BL], fLeft, fMid, fCen, fBottom, nDeep );
		
		pNode->_children[NODE::BR] = new NODE();
		CreateChild( pNode->_children[NODE::BR], fCen, fMid, fRight, fBottom, nDeep );
	}
}

BOOL CTree::IsRect( NODE *pNode, CBoundBox * pBoundBox)
{
	assert(pNode != NULL);

	BOOL bRet = FALSE;
	D3DXVECTOR3 vPos = pBoundBox->GetPosition();
	RECT rect = pNode->GetRect();
	if ( rect.left   < ( vPos.x + pBoundBox->GetSize( CBoundBox::PLUSX ) ) && 
		 rect.right  > ( vPos.x + pBoundBox->GetSize( CBoundBox::MINUSX ) ) && 
		 rect.top    > ( vPos.z + pBoundBox->GetSize( CBoundBox::MINUSZ ) ) && 
		 rect.bottom < ( vPos.z + pBoundBox->GetSize( CBoundBox::PLUSZ ) ) )
		 bRet = TRUE;

	return bRet;
}

BOOL CTree::IsRect( NODE *pNode, D3DXVECTOR3 &vPos)
{
	assert(pNode != NULL);

	BOOL bRet = FALSE;

	if ( pNode->_pos[NODE::TL].x < vPos.x && vPos.x < pNode->_pos[NODE::BR].x )
		if(	pNode->_pos[NODE::TL].z > vPos.z && vPos.z > pNode->_pos[NODE::BR].z )
			bRet = TRUE;

	return bRet;
}

std::vector <CBoundBox*> * CTree::GetVector( NODE *pNode, D3DXVECTOR3 vPos )
{
	assert(pNode != NULL);

	if ( pNode->_children[NODE::TL] == NULL )
	{
		return &pNode->_data;
	}

	if ( IsRect( pNode->_children[NODE::TL], vPos ) )	
		return GetVector( pNode->_children[NODE::TL], vPos );
	if ( IsRect( pNode->_children[NODE::TR], vPos ) )		
		return GetVector( pNode->_children[NODE::TR], vPos );
	if ( IsRect( pNode->_children[NODE::BL], vPos ) )		
		return GetVector( pNode->_children[NODE::BL], vPos );
	if ( IsRect( pNode->_children[NODE::BR], vPos ) )		
		return GetVector( pNode->_children[NODE::BR], vPos );

	return NULL;
}
