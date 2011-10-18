#include "stdafx.h"
#include "ASEParser.h"

VOID ASEParser::Initialize()
{
	m_pKeyword			=	NULL;
	m_pGeomObjectData	=	NULL;
	m_pMaterialData		=	NULL;

	m_iCurrentGeomObjectIndex	=	0;
	m_iCurrentMaterialIndex		=	0;

	InitKeyword();
}

VOID ASEParser::InitKeyword()
{
	m_pKeyword = new ASEKEY[ ASEKEY::END ];

	SetAseKey( m_pKeyword, ASEKEY::ASCIIEXPORT,			L"*3DSMAX_ASCIIEXPORT" );
	SetAseKey( m_pKeyword, ASEKEY::MATERIAL_LIST,		L"*MATERIAL_LIST {" );
	SetAseKey( m_pKeyword, ASEKEY::MATERIAL_COUNT,		L"*MATERIAL_COUNT " );
	SetAseKey( m_pKeyword, ASEKEY::MATERIAL,			L"*MATERIAL" );
	SetAseKey( m_pKeyword, ASEKEY::MAP_DIFFUSE,			L"*MAP_DIFFUSE {" );
	SetAseKey( m_pKeyword, ASEKEY::BITMAP,				L"*BITMAP " );
	SetAseKey( m_pKeyword, ASEKEY::GEOMOBJECT,			L"*GEOMOBJECT {" );
	SetAseKey( m_pKeyword, ASEKEY::NODE_NAME,			L"*NODE_NAME " );
	SetAseKey( m_pKeyword, ASEKEY::NODE_TM,				L"*NODE_TM {" );
	SetAseKey( m_pKeyword, ASEKEY::MESH,				L"*MESH {" );
	SetAseKey( m_pKeyword, ASEKEY::MESH_NUMVERTEX,		L"*MESH_NUMVERTEX " );
	SetAseKey( m_pKeyword, ASEKEY::MESH_NUMFACES,		L"*MESH_NUMFACES " );
	SetAseKey( m_pKeyword, ASEKEY::MESH_NUMTVERTEX,		L"*MESH_NUMTVERTEX " );
	SetAseKey( m_pKeyword, ASEKEY::MESH_VERTEX_LIST,	L"*MESH_VERTEX_LIST {" );
	SetAseKey( m_pKeyword, ASEKEY::MESH_FACE_LIST,		L"*MESH_FACE_LIST {" );
	SetAseKey( m_pKeyword, ASEKEY::MESH_NUMTVERTEX,		L"*MESH_NUMTVERTEX " );
	SetAseKey( m_pKeyword, ASEKEY::MESH_TVERTLIST,		L"*MESH_TVERTLIST {" );
	SetAseKey( m_pKeyword, ASEKEY::MESH_NUMTFACES,		L"*MESH_NUMTVFACES " );
	SetAseKey( m_pKeyword, ASEKEY::MESH_TFACELIST,		L"*MESH_TFACELIST {" );
	SetAseKey( m_pKeyword, ASEKEY::MESH_NORMALS,		L"*MESH_NORMALS {" );
	SetAseKey( m_pKeyword, ASEKEY::MESH_VERTEXNORMAL,	L"*MESH_VERTEXNORMAL" );
	SetAseKey( m_pKeyword, ASEKEY::MATERIAL_REF,		L"*MATERIAL_REF" );
}
VOID ASEParser::Release()
{
	if( m_pKeyword != NULL )
		delete[] m_pKeyword;
	Cleanup();
}

VOID ASEParser::Cleanup()
{
	m_iCurrentGeomObjectIndex	=	0;
	m_iCurrentMaterialIndex		=	0;

	if( m_pGeomObjectData != NULL )
		delete[] m_pGeomObjectData;
	if( m_pMaterialData != NULL )
		delete[] m_pMaterialData;
}

BOOL ASEParser::LoadFile( LPWSTR _FileName )
{
	Cleanup();

	
	//CDebugConsole::GetInstance()->Messagef( L"ASEParser\n" );
	//CDebugConsole::GetInstance()->Message( _FileName );
	//CDebugConsole::GetInstance()->Messagef( L"\n" );

	FILE*	fp;
	fp = _wfopen( _FileName, L"rt" );
	TCHAR	sLine[ 516 ];

	ReadLineFromFile( fp, sLine );
	if( !CheckFile( fp, sLine ) )return FALSE;
	
	CountGeometry( fp );

	while( !feof( fp ) )
	{
		ReadLineFromFile( fp, sLine );
		
		BeginMaterialList( fp, sLine );
		BeginGeomObject( fp, sLine );
	}

	TFaceProcessing();
		
	fclose( fp );

	return TRUE;
}

BOOL ASEParser::GetObjectData( const INT _Index, MATERIALDATA& _OutMaterialData, GEOMOBJECTDATA& _OutGeomObjectData )
{
	if( m_pMaterialData[ m_pGeomObjectData[ _Index ].MaterialREF ].pTextureFileName != NULL )
	{
		_OutMaterialData.pTextureFileName = new TCHAR[ 1024 ];
		lstrcpy( _OutMaterialData.pTextureFileName, m_pMaterialData[ m_pGeomObjectData[ _Index ].MaterialREF ].pTextureFileName );
	}

	_OutGeomObjectData.iNumVertex	=	m_pGeomObjectData[ _Index ].iNumVertex;
	_OutGeomObjectData.iNumIndex	=	m_pGeomObjectData[ _Index ].iNumIndex;
	_OutGeomObjectData.MaterialREF	=	m_pGeomObjectData[ _Index ].MaterialREF;

	if( m_pGeomObjectData[ _Index ].pNodeName != NULL )
	{
		_OutGeomObjectData.pNodeName = new TCHAR[ 1024 ];
		lstrcpy( _OutGeomObjectData.pNodeName, m_pGeomObjectData[ _Index ].pNodeName );
	}
	
	if( _OutGeomObjectData.pVertex != NULL )
		delete[] _OutGeomObjectData.pVertex;
	_OutGeomObjectData.pVertex	=	new VERTEX[ m_pGeomObjectData[ _Index ].iNumVertex ];
	for( INT i=0 ; i<m_pGeomObjectData[ _Index ].iNumVertex ; i++ )
	{
		_OutGeomObjectData.pVertex[ i ].pos			= m_pGeomObjectData[ _Index ].pVertex[ i ].pos;
		_OutGeomObjectData.pVertex[ i ].normal		= m_pGeomObjectData[ _Index ].pVertex[ i ].normal;
		_OutGeomObjectData.pVertex[ i ].texcoord	= m_pGeomObjectData[ _Index ].pVertex[ i ].texcoord;
	}
	
	if( _OutGeomObjectData.pIndex != NULL )
		delete[] _OutGeomObjectData.pIndex;
	_OutGeomObjectData.pIndex	=	new INDEX[ m_pGeomObjectData[ _Index ].iNumIndex ];
	for( INT i=0 ; i<m_pGeomObjectData[ _Index ].iNumIndex ; i++ )
	{
		_OutGeomObjectData.pIndex[ i ]._0 = m_pGeomObjectData[ _Index ].pIndex[ i ]._0;
		_OutGeomObjectData.pIndex[ i ]._1 = m_pGeomObjectData[ _Index ].pIndex[ i ]._1;
		_OutGeomObjectData.pIndex[ i ]._2 = m_pGeomObjectData[ _Index ].pIndex[ i ]._2;
	}

	return TRUE;
}

INT ASEParser::GetNumObject()
{
	return m_iNumGeomObject;
}
BOOL ASEParser::CountGeometry( FILE* _fp )
{
	TCHAR	sLine[ 516 ];
	INT		nCount = 0;

	while( !feof( _fp ) )
	{
		ReadLineFromFile( _fp, sLine );
		
		if( 0 == wcsnicmp( sLine, m_pKeyword[ ASEKEY::GEOMOBJECT ].sKey, m_pKeyword[ ASEKEY::GEOMOBJECT ].iKey ) )
			nCount++;

	}
	m_iNumGeomObject = nCount;
	//CDebugConsole::GetInstance()->Messagef( L"GEOMOBJECT_COUNT : %d\n", m_iNumGeomObject );

	m_pGeomObjectData = new GEOMOBJECTDATA[ m_iNumGeomObject ];

	fseek( _fp, 0, SEEK_SET );

	return TRUE;
}

BOOL ASEParser::CheckFile( FILE* _fp, LPWSTR _sLine )
{
	if( 0 == wcsnicmp( _sLine, m_pKeyword[ ASEKEY::ASCIIEXPORT ].sKey, m_pKeyword[ ASEKEY::ASCIIEXPORT ].iKey ) )
	{
		//CDebugConsole::GetInstance()->Messagef( L"CHECK FILE : SUCCESS \n" );
		return TRUE;
	}
	
	//CDebugConsole::GetInstance()->Messagef( L"CHECK FILE : FAIL \n" );

	MessageBox( NULL, L"ASE File이 아닙니다", NULL, MB_OK );

	return FALSE;
}

BOOL ASEParser::BeginMaterialList( FILE* _fp, LPWSTR _sLine )
{
	if( 0 == wcsnicmp( _sLine, m_pKeyword[ ASEKEY::MATERIAL_LIST ].sKey, m_pKeyword[ ASEKEY::MATERIAL_LIST ].iKey ) )
	{
		while( !feof( _fp ) )
		{
			ReadLineFromFile( _fp, _sLine );

			if( L'}' == _sLine[ 0 ] )
				break;
			
			GetMaterialCount( _sLine );
			
			BeginMaterial( _fp, _sLine );
		}
		return TRUE;
	}
	return FALSE;
}

BOOL ASEParser::GetMaterialCount( LPWSTR _sLine )
{
	if( 0 == wcsnicmp( _sLine, m_pKeyword[ ASEKEY::MATERIAL_COUNT ].sKey, m_pKeyword[ ASEKEY::MATERIAL_COUNT ].iKey ) )
	{
		INT nCount = 0;
		swscanf( _sLine, L"%*s %d", &nCount );

		m_iNumMaterial = nCount;
		//CDebugConsole::GetInstance()->Messagef( L"*MATERIAL_COUNT %d\n", m_iNumMaterial );

		m_pMaterialData = new MATERIALDATA[ m_iNumMaterial ];

		return TRUE;
	}

	return FALSE;
}
BOOL ASEParser::BeginMaterial( FILE* _fp, LPWSTR _sLine )
{
	TCHAR	str[ 1024 ];
	INT		iLen = 0;
	wsprintf( str, L"%s %d {", m_pKeyword[ ASEKEY::MATERIAL ].sKey, m_iCurrentMaterialIndex );
	iLen  = lstrlen( str );

	if(  0 == wcsnicmp( _sLine, str, iLen ) )
	{
		while( !feof( _fp ) )
		{
			ReadLineFromFile( _fp, _sLine );

			if( L'}' == _sLine[ 0 ] )
				break;

			BeginMapDiffuse( _fp, _sLine );

		}
		m_iCurrentMaterialIndex++;

		return TRUE;
	}

	return FALSE;
}
BOOL ASEParser::BeginMapDiffuse( FILE* _fp, LPWSTR _sLine )
{
	if(  0 == wcsnicmp( _sLine, m_pKeyword[ ASEKEY::MAP_DIFFUSE ].sKey, m_pKeyword[ ASEKEY::MAP_DIFFUSE ].iKey ) )
	{
		while( !feof( _fp ) )
		{
			ReadLineFromFile( _fp, _sLine );

			if( L'}' == _sLine[ 0 ] )
				break;

			GetBitmap( _sLine );
		}
		return TRUE;
	}

	return FALSE;
}
BOOL ASEParser::GetBitmap( LPWSTR _sLine )
{
	if(  0 == wcsnicmp( _sLine, m_pKeyword[ ASEKEY::BITMAP ].sKey, m_pKeyword[ ASEKEY::BITMAP ].iKey ) )
	{
		TCHAR	Name[ 516 ];
		StrQuot( Name, _sLine );

		m_pMaterialData[ m_iCurrentMaterialIndex ].pTextureFileName = new TCHAR[ 1024 ];


		lstrcpy( m_pMaterialData[ m_iCurrentMaterialIndex ].pTextureFileName, Name );

		TCHAR	str[ 516 ];
		wsprintf( str, L"*BITMAP : %s\n", Name );
		//CDebugConsole::GetInstance()->Message( str );
		
		return TRUE;
	}

	return FALSE;
}

BOOL ASEParser::BeginGeomObject( FILE* _fp, LPWSTR _sLine )
{
	if( 0 == wcsnicmp( _sLine, m_pKeyword[ ASEKEY::GEOMOBJECT ].sKey, m_pKeyword[ ASEKEY::GEOMOBJECT ].iKey ) )
	{
		while( !feof( _fp ) )
		{
			ReadLineFromFile( _fp, _sLine );

			if( L'}' == _sLine[ 0 ] )
				break;

			GetNodeName( _sLine );

			BeginNodeTM( _fp, _sLine );
			BeginMesh( _fp, _sLine );

			GetMaterialRef( _sLine );
			
		}
		
		m_iCurrentGeomObjectIndex++;

		return TRUE;
	}

	return FALSE;
}
BOOL ASEParser::GetNodeName( LPWSTR _sLine )
{
	if( 0 == wcsnicmp( _sLine, m_pKeyword[ ASEKEY::NODE_NAME ].sKey, m_pKeyword[ ASEKEY::NODE_NAME ].iKey ) )
	{
		TCHAR	Name[ 516 ];
		StrQuot( Name, _sLine );

		m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].pNodeName = new TCHAR[ 1024 ];

		lstrcpy( m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].pNodeName, Name );

		TCHAR	str[ 516 ];
		wsprintf( str, L"NODE_NAME : %s\n", m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].pNodeName );
		//CDebugConsole::GetInstance()->Message( str );

		return TRUE;
	}//	End keyword[ 1 ]

	return FALSE;
}
BOOL ASEParser::BeginNodeTM( FILE* _fp, LPWSTR _sLine )
{
	if( 0 == wcsnicmp( _sLine, m_pKeyword[ ASEKEY::NODE_TM ].sKey, m_pKeyword[ ASEKEY::NODE_TM ].iKey ) )
	{
		while( !feof( _fp ) )
		{
			ReadLineFromFile( _fp, _sLine );

			if( '}' == _sLine[ 0 ] )
				break;
		}
		return TRUE;
	}

	return FALSE;
}
BOOL ASEParser::BeginMesh( FILE* _fp, LPWSTR _sLine )
{
	if(  0 == wcsnicmp( _sLine, m_pKeyword[ ASEKEY::MESH ].sKey, m_pKeyword[ ASEKEY::MESH ].iKey ) )
	{
		while( !feof( _fp ) )
		{
			ReadLineFromFile( _fp, _sLine );

			if( L'}' == _sLine[ 0 ] )
				break;
			
			GetMeshNumVertex( _sLine );
			GetMeshNumFaces( _sLine );

			BeginMeshVertexList( _fp, _sLine );
			BeginMeshFaceList( _fp, _sLine );
			
			GetMeshNumTVertex( _sLine );
			BeginMeshTVertexList( _fp, _sLine );

			GetMeshNumTFaces( _sLine );
			BeginMeshTFaceList( _fp, _sLine );

			BeginMeshNormals( _fp, _sLine );
			
		}
		return TRUE;
	}
	return FALSE;
}

BOOL ASEParser::GetMeshNumVertex( LPWSTR _sLine )
{
	if(  0 == wcsnicmp( _sLine, m_pKeyword[ ASEKEY::MESH_NUMVERTEX ].sKey, m_pKeyword[ ASEKEY::MESH_NUMVERTEX ].iKey ) )
	{
		INT NumVertex;
		swscanf( _sLine, L"%*s %d", &NumVertex );

		m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].iNumMeshVertex	=	NumVertex;

		//CDebugConsole::GetInstance()->Messagef( L"MESH_NUMVERTEX : %d\n", m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].iNumMeshVertex );
		
		return TRUE;
	}//	End Keyword[ 3 ]

	return FALSE;
}
BOOL ASEParser::GetMeshNumFaces( LPWSTR _sLine )
{
	if(  0 == wcsnicmp( _sLine, m_pKeyword[ ASEKEY::MESH_NUMFACES ].sKey, m_pKeyword[ ASEKEY::MESH_NUMFACES ].iKey ) )
	{
		INT NumIndex = 0;
		swscanf( _sLine, L"%*s %d", &NumIndex );

		m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].iNumMeshFace	=	NumIndex;

		//CDebugConsole::GetInstance()->Messagef( L"MESH_NUMINDEX : %d\n", m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].iNumMeshFace );

		return TRUE;
	}//	End Keyword[ 4 ]

	return FALSE;
}

BOOL ASEParser::BeginMeshVertexList( FILE* _fp, LPWSTR _sLine )
{
	if( 0 == wcsnicmp( _sLine, m_pKeyword[ ASEKEY::MESH_VERTEX_LIST ].sKey, m_pKeyword[ ASEKEY::MESH_VERTEX_LIST ].iKey ) )
	{
		m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].pMeshVertex = new VERTEX[ m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].iNumMeshVertex ];

		while( !feof( _fp ) )
		{
			ReadLineFromFile( _fp, _sLine );
			
			if( '}' == _sLine[ 0 ] )
				break;

			INT		nIndex = 0;
			FLOAT	x = 0.0f, y = 0.0f, z = 0.0f;
			swscanf( _sLine, L"%*s %d %f %f %f", &nIndex, &x, &y, &z );

			m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].pMeshVertex[ nIndex ].pos.x = x;
			m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].pMeshVertex[ nIndex ].pos.y = z;
			m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].pMeshVertex[ nIndex ].pos.z = y;

		}//	End while

		return TRUE;
	}

	return FALSE;
}
BOOL ASEParser::BeginMeshFaceList( FILE* _fp, LPWSTR _sLine )
{
	if( 0 == wcsnicmp( _sLine, m_pKeyword[ ASEKEY::MESH_FACE_LIST ].sKey, m_pKeyword[ ASEKEY::MESH_FACE_LIST ].iKey ) )
	{
		m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].pMeshFace = new INDEX[ m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].iNumMeshFace ];

		while( !feof( _fp ) )
		{
			ReadLineFromFile( _fp, _sLine );
			
			if( L'}' == _sLine[ 0 ] )
				break;

			INT nIndex = 0;
			INT _0 = 0, _1 = 0, _2 = 0;
			swscanf( _sLine, L"%*s %d: %*s %d %*s %d %*s %d", &nIndex, &_0, &_1, &_2 );

			////CDebugConsole::GetInstance()->Messagef( L" MESH_FACE : %d A : %d B : %d C : %d\n", nIndex, _0, _1, _2 );

			m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].pMeshFace[ nIndex ]._0 = _0;
			m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].pMeshFace[ nIndex ]._1 = _2;
			m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].pMeshFace[ nIndex ]._2 = _1;
		}//	End While

		return TRUE;
	}//	End Keyword[ 7 ]

	return FALSE;
}
BOOL ASEParser::GetMeshNumTVertex( LPWSTR _sLine )
{
	if( 0 == wcsnicmp( _sLine, m_pKeyword[ ASEKEY::MESH_NUMTVERTEX ].sKey, m_pKeyword[ ASEKEY::MESH_NUMTVERTEX ].iKey ) )
	{
		INT NumTVertex = 0;
		swscanf( _sLine, L"%*s %d", &NumTVertex );

		m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].iNumMeshTVertex	=	NumTVertex;
		m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].pMeshTVertex		=	new VERTEX[ NumTVertex ];

		////CDebugConsole::GetInstance()->Messagef( L"MESH_NUMTVERTEX : %d\n", m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].iNumMeshTVertex );

		return TRUE;
	}
	return FALSE;
}
BOOL ASEParser::BeginMeshTVertexList( FILE* _fp, LPWSTR _sLine )
{
	if( 0 == wcsnicmp( _sLine, m_pKeyword[ ASEKEY::MESH_TVERTLIST ].sKey, m_pKeyword[ ASEKEY::MESH_TVERTLIST ].iKey ) )
	{
		while( !feof( _fp ) )
		{
			ReadLineFromFile( _fp, _sLine );

			if( L'}' == _sLine[ 0 ] )
				break;

			INT		nIndex = 0;
			FLOAT	u = 0.0f, v = 0.0f, w = 0.0f;
			swscanf( _sLine, L"%*s %d  %f %f %f", &nIndex, &u, &v, &w );

			////CDebugConsole::GetInstance()->Messagef( L" MESH_FACE : %d A : %d B : %d C : %d\n", nIndex, _0, _1, _2 );

			m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].pMeshTVertex[ nIndex ].texcoord.x = u;
			m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].pMeshTVertex[ nIndex ].texcoord.y = 1.0f - v;
			
		}//	End While

		return TRUE;
	}//	End Keyword[ 7 ]

	return FALSE;

}
BOOL ASEParser::GetMeshNumTFaces( LPWSTR _sLine )
{
	if( 0 == wcsnicmp( _sLine, m_pKeyword[ ASEKEY::MESH_NUMTFACES ].sKey, m_pKeyword[ ASEKEY::MESH_NUMTFACES ].iKey ) )
	{	
		INT NumTFaces = 0;
		swscanf( _sLine, L"%*s %d", &NumTFaces );

		m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].iNumMeshTFace	=	NumTFaces;
		m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].pMeshTFace		=	new INDEX[ NumTFaces ];

		//CDebugConsole::GetInstance()->Messagef( L"MESH_NUMTFACES : %d\n", m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].iNumMeshTFace );

		return TRUE;
	}
	return FALSE;
}
BOOL ASEParser::BeginMeshTFaceList( FILE* _fp, LPWSTR _sLine )
{
	if( 0 == wcsnicmp( _sLine, m_pKeyword[ ASEKEY::MESH_TFACELIST ].sKey, m_pKeyword[ ASEKEY::MESH_TFACELIST ].iKey ) )
	{
		while( !feof( _fp ) )
		{
			ReadLineFromFile( _fp, _sLine );

			if( L'}' == _sLine[ 0 ] )
				break;

			INT nTIndex = 0;
			INT _0 = 0, _1 = 0, _2 = 0;
			swscanf( _sLine, L"%*s %d %d %d %d", &nTIndex, &_0, &_1, &_2 );

			////CDebugConsole::GetInstance()->Messagef( L" MESH_FACE : %d A : %d B : %d C : %d\n", nIndex, _0, _1, _2 );

			m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].pMeshTFace[ nTIndex ]._0 = _0;
			m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].pMeshTFace[ nTIndex ]._1 = _2;
			m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].pMeshTFace[ nTIndex ]._2 = _1;
		}
		return TRUE;
	}

	return FALSE;
}

BOOL ASEParser::BeginMeshNormals( FILE* _fp, LPWSTR _sLine )
{
	if( 0 == wcsnicmp( _sLine, m_pKeyword[ ASEKEY::MESH_NORMALS ].sKey, m_pKeyword[ ASEKEY::MESH_NORMALS ].iKey ) )
	{
		while( !feof( _fp ) )
		{
			ReadLineFromFile( _fp, _sLine );

			if( L'}' == _sLine[ 0 ] )
				break;

			GetMeshVertexNormal( _sLine );
		}
		return TRUE;
	}

	return FALSE;
}
BOOL ASEParser::GetMeshVertexNormal( LPWSTR _sLine )
{
	if( 0 == wcsnicmp( _sLine, m_pKeyword[ ASEKEY::MESH_VERTEXNORMAL ].sKey, m_pKeyword[ ASEKEY::MESH_VERTEXNORMAL ].iKey ) )
	{
		INT		nIndex = 0;
		FLOAT	x = 0.0f, y = 0.0f, z = 0.0f;
		
		swscanf( _sLine, L"%*s %d %f %f %f", &nIndex, &x, &y, &z );

		m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].pMeshVertex[ nIndex ].normal.x  = x;
		m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].pMeshVertex[ nIndex ].normal.y  = z;
		m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].pMeshVertex[ nIndex ].normal.z  = y;

		return TRUE;
	}

	return FALSE;
}

BOOL ASEParser::GetMaterialRef( LPWSTR _sLine )
{
	if( 0 == wcsnicmp( _sLine, m_pKeyword[ ASEKEY::MATERIAL_REF ].sKey, m_pKeyword[ ASEKEY::MATERIAL_REF ].iKey ) )
	{
		INT	iRef = 0;
		swscanf( _sLine, L"%*s %d", &iRef );
		
		m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].MaterialREF = iRef;

		return TRUE;
	}

	return FALSE;
}

BOOL ASEParser::TFaceProcessing()
{
	for( INT i=0 ; i<m_iNumGeomObject ; i++ )
	{
		LPGEOMOBJECTDATA pGeomObjectData = &m_pGeomObjectData[ i ];

		if( pGeomObjectData->pMeshTFace == NULL )
			continue;

		//	*MESH_NUMTVERTEX 갯수만큼 할당
		pGeomObjectData->pVertex = new VERTEX[ pGeomObjectData->iNumMeshTVertex ];

		//	*MESH_TVERTLIST 의 UV값을 대입
		for( INT j=0 ; j<pGeomObjectData->iNumMeshTVertex ; j++ )
		{
			pGeomObjectData->pVertex[ j ].texcoord.x = pGeomObjectData->pMeshTVertex[ j ].texcoord.x;
			pGeomObjectData->pVertex[ j ].texcoord.y = pGeomObjectData->pMeshTVertex[ j ].texcoord.y;
		}

		//	새로 만든 pVertex에 이전 값이 저장되어 있는 pGeomObjectData의 정점값을 옮긴다
		for( INT j=0 ; j<pGeomObjectData->iNumMeshFace ; j++ )
		{
			INT nTVertex	= 0;
			INT nVertex		= 0;

			//	nTVertex == *MESH_TFACELIST에 저장되어 있는 3개의 인덱스값
			nTVertex	= pGeomObjectData->pMeshTFace[ j ]._0;
			//	nVertex == *MESH_FACE_LIST에 저장되어 있는 3개의 인덱스값
			nVertex		= pGeomObjectData->pMeshFace[ j ]._0;

			pGeomObjectData->pVertex[ nTVertex ].pos	= pGeomObjectData->pMeshVertex[ nVertex ].pos;
			pGeomObjectData->pVertex[ nTVertex ].normal	= pGeomObjectData->pMeshVertex[ nVertex ].normal;

			nTVertex	= pGeomObjectData->pMeshTFace[ j ]._1;
			nVertex		= pGeomObjectData->pMeshFace[ j ]._1;

			pGeomObjectData->pVertex[ nTVertex ].pos	= pGeomObjectData->pMeshVertex[ nVertex ].pos;
			pGeomObjectData->pVertex[ nTVertex ].normal	= pGeomObjectData->pMeshVertex[ nVertex ].normal;

			nTVertex	= pGeomObjectData->pMeshTFace[ j ]._2;
			nVertex		= pGeomObjectData->pMeshFace[ j ]._2;

			pGeomObjectData->pVertex[ nTVertex ].pos	= pGeomObjectData->pMeshVertex[ nVertex ].pos;
			pGeomObjectData->pVertex[ nTVertex ].normal	= pGeomObjectData->pMeshVertex[ nVertex ].normal;

		}
		
		pGeomObjectData->iNumVertex = pGeomObjectData->iNumMeshTVertex;

		pGeomObjectData->pIndex  = new INDEX[ pGeomObjectData->iNumMeshTFace ];
		memcpy( pGeomObjectData->pIndex, pGeomObjectData->pMeshTFace, sizeof( INDEX ) * pGeomObjectData->iNumMeshTFace );
		pGeomObjectData->iNumIndex = pGeomObjectData->iNumMeshTFace;
	}

	return TRUE;
}