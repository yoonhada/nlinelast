#include "stdafx.h"

#include "Astar.h"


Astar::Astar()
{
}

Astar::~Astar()
{
	for( INT i=0; i<m_iWidth; i++ )
	{
		SAFE_DELETE_ARRAY( m_Map[i] );
	}
	SAFE_DELETE_ARRAY( m_Map );

	SAFE_DELETE( m_pOpen );
}


VOID Astar::Initialize( INT a_iWidth, INT a_iHeight, INT* a_Map )
{
	m_iWidth = a_iWidth;
	m_iHeight = a_iHeight;

	m_Map = new PathMap*[a_iWidth];

	for( INT i=0; i<a_iWidth; i++ )
	{
		m_Map[i] = new PathMap[a_iHeight];
	}

	m_pOpen = new TreeRoot;
	m_pOpen->root = NULL;

	// 12시 부터 시계방향
	m_Dir[0][0] = 0;	m_Dir[0][1] = -1;
	m_Dir[1][0] = 1;	m_Dir[1][1] = -1;
	m_Dir[2][0] = 1;	m_Dir[2][1] = 0;
	m_Dir[3][0] = 1;	m_Dir[3][1] = 1;
	m_Dir[4][0] = 0;	m_Dir[4][1] = 1;
	m_Dir[5][0] = -1;	m_Dir[5][1] = -1;
	m_Dir[6][0] = -1;	m_Dir[6][1] = 0;
	m_Dir[7][0] = -1;	m_Dir[7][1] = 1;

	createMap( a_Map );
	clearMap();
}


VOID Astar::createMap( INT* a_Map )
{
	for( INT i=0; i<m_iWidth; i++ )
	{
		for( INT j=0; j<m_iHeight; j++ )
		{
			m_Map[i][j].map = a_Map[i + j * m_iWidth];
		}
	}
}


VOID Astar::clearMap()
{
	for( INT i=0; i<m_iWidth; i++ )
	{
		for( INT j=0; j<m_iHeight; j++ )
		{
			if( m_Map[i][j].map != 1 )
			{
				m_Map[i][j].visit = FALSE;
				m_Map[i][j].map = 0;
			}
		}
	}
}


VOID Astar::setPath( INT a_iSx, INT a_iSy, INT a_iEx, INT a_iEy, PathNode* a_pPath )
{
	PathNode* temp = a_pPath;

	while( temp != NULL )
	{
		m_Map[temp->x][temp->y].map = 2;
		temp = temp->next;
	}

	m_Map[a_iSx][a_iSy].map = 10;
	m_Map[a_iEx][a_iEy].map = 20;
}


BOOL Astar::isLoad( INT a_iX, INT a_iY )
{
	if( a_iX < 0 || a_iX >= m_iWidth || a_iY < 0 || a_iY >= m_iHeight )
	{
		return FALSE;
	}

	if( m_Map[a_iX][a_iY].map != 0 )
	{
		return FALSE;
	}

	return TRUE;
}


/*TREE*/
TreeNode* Astar::createNode( INT a_iX, INT a_iY )
{
	TreeNode* node = new TreeNode;
	node->x = a_iX;
	node->y = a_iY;
	node->left = NULL;
	node->right = NULL;

	return node;
}


VOID Astar::removeTree( TreeRoot* a_pRoot )
{
	deleteNode( a_pRoot->root );
}


VOID Astar::deleteNode( TreeNode* a_pNode )
{
	if( a_pNode == NULL )
	{
		return;
	}
	else
	{
		deleteNode( a_pNode->left );
		deleteNode( a_pNode->right );

		SAFE_DELETE( a_pNode );
	}
}


VOID Astar::insert( TreeRoot* a_pRoot, TreeNode* a_pNode )
{
	a_pRoot->root = insertNode( a_pRoot->root, a_pNode );
}


TreeNode* Astar::insertNode( TreeNode* a_p, TreeNode* a_pNode )
{
	if( a_p == NULL )
	{
		return a_pNode;
	}
	else if( m_Map[a_p->x][a_p->y].f > m_Map[a_pNode->x][a_pNode->y].f )
	{
		a_p->left = insertNode( a_p->left, a_pNode );
		return a_p;
	}
	else
	{
		a_p->right = insertNode( a_p->right, a_pNode );
		return a_p;
	}
}


/*PATH*/
PathNode* Astar::createPath( INT a_iX, INT a_iY )
{
	PathNode *path = new PathNode;
	path->x = a_iX;
	path->y = a_iY;
	path->next = NULL;

	return path;
}


PathNode* Astar::getPath( INT a_iSx, INT a_iSy, INT a_iEx, INT a_iEy )
{
	PathNode* path = NULL;
	PathNode* temp = NULL;
	INT x = a_iEx;
	INT y = a_iEy;
	INT tx = x;
	INT ty = y;

	if( m_Map[x][y].cameX < 0 || m_Map[x][y].cameY < 0 )
	{
		return NULL;
	}

	while( a_iSx != x || a_iSy != y )
	{
		temp = createPath( x, y );
		if( temp == NULL )
		{
			return NULL;
		}
		temp->next = path;
		path = temp;
		tx = x;
		ty = y;
		x = m_Map[tx][ty].cameX;
		y = m_Map[tx][ty].cameY;
	}

	return path;
}


VOID Astar::removePath( PathNode* a_pPath )
{
	if( a_pPath == NULL )
	{
		return;
	}

	removePath( a_pPath->next );

	SAFE_DELETE( a_pPath );
}


/*ASTAR*/
BOOL Astar::getminimum( INT min[] )
{
	if( m_pOpen->root == NULL )
	{
		return FALSE;
	}
	else if( m_pOpen->root->left == NULL )
	{
		TreeNode* find = m_pOpen->root;
		m_pOpen->root = m_pOpen->root->right;

		min[0] = find->x;
		min[1] = find->y;
		SAFE_DELETE( find );

		return TRUE;
	}

	TreeNode* p = m_pOpen->root;
	while( p->left->left != NULL )
	{
		p = p->left;
	}
	TreeNode* find = p->left;
	p->left = p->left->right;

	min[0] = find->x;
	min[1] = find->y;
	SAFE_DELETE( find );

	return TRUE;
}


VOID Astar::openMap( INT bx, INT by, INT vx, INT vy )
{
	if( vx < 0 || vy < 0 || vx >= m_iWidth || vy >= m_iHeight )
	{
		return;
	}

	if( m_Map[vx][vy].map != 0 )
	{
		return;
	}

	FLOAT g = getDistG( bx, by, vx, vy );
	FLOAT h = getDistH( vx, vy );
	FLOAT f = g + h;

	if( !m_Map[vx][vy].visit || m_Map[vx][vy].f > f )
	{
		if( !m_Map[vx][vy].visit )
		{
			m_Map[vx][vy].visit = TRUE;
		}

		m_Map[vx][vy].cameX = bx;
		m_Map[vx][vy].cameY = by;
		m_Map[vx][vy].g = g;
		m_Map[vx][vy].h = h;
		m_Map[vx][vy].f = f;

		m_Map[vx][vy].map = 3;

		insert( m_pOpen, createNode( vx, vy ) );
	}
}


PathNode* Astar::findPath( INT a_iSx, INT a_iSy, INT a_iEx, INT a_iEy )
{
	PathNode* path = NULL;

	if( a_iSx == a_iEx && a_iSy == a_iEy )
	{
		return path;
	}

	m_Start[0] = a_iSx;
	m_Start[1] = a_iSy;

	m_End[0] = a_iEx;
	m_End[1] = a_iEy;

	INT x = a_iSx;
	INT y = a_iSy;

	m_Map[x][y].visit = TRUE;

	m_Map[x][y].cameX = -1;
	m_Map[x][y].cameY = -1;
	m_Map[x][y].g = 0;
	m_Map[x][y].h = getDistH( x,y );
	m_Map[x][y].f = getDistF( x,y );
	insert( m_pOpen, createNode( x,y ) );

	INT v[2];
	while( getminimum( v ) )
	{
		x = v[0];
		y = v[1];
		m_Map[x][y].map = 4;
		if( x == m_End[0] && y == m_End[1] )
		{
			break;
		}

		for( INT i=0; i<8; ++i )
		{
			openMap( x, y, x + m_Dir[i][0], y + m_Dir[i][1] );
		}
	}

	m_Map[a_iSx][a_iSy].map = 10;
	m_Map[a_iEx][a_iEy].map = 20;

	removeTree( m_pOpen );
	m_pOpen->root = NULL;

	return getPath( m_Start[0], m_Start[1], m_End[0], m_End[1] );
}


FLOAT Astar::getDistG( INT bx, INT by, INT vx, INT vy )
{
	return m_Map[bx][by].g + sqrt( FLOAT( ( bx - vx ) * ( bx - vx ) + ( by - vy ) * ( by - vy ) ) );
}


FLOAT Astar::getDistH( INT x, INT y )
{
	return FLOAT( abs( x - m_End[0] ) + abs( y - m_End[1] ) );
}


FLOAT Astar::getDistF( INT x, INT y )
{
	return m_Map[x][y].g + m_Map[x][y].h;
}
