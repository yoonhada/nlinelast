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


VOID Astar::setPath( INT sx, INT sy, INT ex, INT ey, PathNode* path )
{
	PathNode* temp = path;

	while( temp != NULL )
	{
		m_Map[temp->x][temp->y].map = 2;
		temp = temp->next;
	}

	m_Map[sx][sy].map = 10;
	m_Map[ex][ey].map = 20;
}


BOOL Astar::isLoad( INT x, INT y )
{
	if( x < 0 || x >= m_iWidth || y < 0 || y >= m_iHeight )
	{
		return FALSE;
	}

	if( m_Map[x][y].map != 0 )
	{
		return FALSE;
	}

	return TRUE;
}


/*TREE*/
TreeNode* Astar::createNode( INT x, INT y )
{
	TreeNode* node = new TreeNode;
	node->x = x;
	node->y = y;
	node->left = NULL;
	node->right = NULL;

	return node;
}


VOID Astar::removeTree( TreeRoot* root )
{
	deleteNode( root->root );
}


VOID Astar::deleteNode( TreeNode *node )
{
	if( node == NULL )
	{
		return;
	}
	else
	{
		deleteNode( node->left );
		deleteNode( node->right );

		SAFE_DELETE( node );
	}
}


VOID Astar::insert( TreeRoot* root, TreeNode* node )
{
	root->root = insertNode( root->root, node );
}


TreeNode* Astar::insertNode( TreeNode *p, TreeNode *node )
{
	if( p == NULL )
	{
		return node;
	}
	else if( m_Map[p->x][p->y].f > m_Map[node->x][node->y].f )
	{
		p->left = insertNode( p->left, node );
		return p;
	}
	else
	{
		p->right = insertNode( p->right, node );
		return p;
	}
}


/*PATH*/
PathNode* Astar::createPath( INT x, INT y )
{
	PathNode *path = new PathNode;
	path->x = x;
	path->y = y;
	path->next = NULL;

	return path;
}


PathNode* Astar::getPath( INT sx, INT sy, INT ex, INT ey )
{
	PathNode* path = NULL;
	PathNode* temp = NULL;
	INT x = ex;
	INT y = ey;
	INT tx = x;
	INT ty = y;

	if( m_Map[x][y].cameX < 0 || m_Map[x][y].cameY < 0 )
	{
		return NULL;
	}

	while( sx != x || sy != y )
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


VOID Astar::removePath( PathNode *path )
{
	if( path == NULL )
	{
		return;
	}

	removePath( path->next );

	SAFE_DELETE( path );
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


PathNode* Astar::findPath( INT sx, INT sy, INT ex, INT ey )
{
	PathNode* path = NULL;

	if( sx == ex && sy == ey )
	{
		return path;
	}

	m_Start[0] = sx;
	m_Start[1] = sy;

	m_End[0] = ex;
	m_End[1] = ey;

	INT x = sx;
	INT y = sy;

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

	m_Map[sx][sy].map = 10;
	m_Map[ex][ey].map = 20;

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
