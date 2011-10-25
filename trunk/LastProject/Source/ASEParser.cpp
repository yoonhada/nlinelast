#include "stdafx.h"
#include "ASEParser.h"

VOID ASEParser::Initialize()
{
	m_pKeyword			=	NULL;
	m_pSceneData		=	NULL;
	m_pMaterialData		=	NULL;
	m_pGeomObjectData	=	NULL;
	
	m_iCurrentGeomObjectIndex	=	0;
	m_iCurrentMaterialIndex		=	0;

	InitKeyword();

	//	Init AnimationData
	m_aniBase.iStartFrame		= 0;
	m_aniBase.iEndFrame			= 0;
	m_aniBase.dID				= 0;
	m_aniBase.bLoop				= TRUE;

	m_aniCurrent.iStartFrame	= 0;
	m_aniCurrent.iEndFrame		= 0;
	m_aniCurrent.dID			= 0;
	m_aniCurrent.bLoop			= FALSE;
	//	End
}

VOID ASEParser::InitKeyword()
{	
	m_pKeyword = new ASEKEY[ ASEKEY::END ];

	SetAseKey( m_pKeyword, ASEKEY::ASCIIEXPORT,			L"*3DSMAX_ASCIIEXPORT" );

	SetAseKey( m_pKeyword, ASEKEY::SCENE,				L"*SCENE {" );
	SetAseKey( m_pKeyword, ASEKEY::SCENE_FIRSTFRAME,	L"*SCENE_FIRSTFRAME " );
	SetAseKey( m_pKeyword, ASEKEY::SCENE_LASTFRAME,		L"*SCENE_LASTFRAME " );
	SetAseKey( m_pKeyword, ASEKEY::SCENE_FRAMESPEED,	L"*SCENE_FRAMESPEED " );
	SetAseKey( m_pKeyword, ASEKEY::SCENE_TICKSPERFRAME,	L"*SCENE_TICKSPERFRAME " );

	SetAseKey( m_pKeyword, ASEKEY::MATERIAL_LIST,		L"*MATERIAL_LIST {" );
	SetAseKey( m_pKeyword, ASEKEY::MATERIAL_COUNT,		L"*MATERIAL_COUNT " );
	SetAseKey( m_pKeyword, ASEKEY::MATERIAL,			L"*MATERIAL" );
	SetAseKey( m_pKeyword, ASEKEY::MAP_DIFFUSE,			L"*MAP_DIFFUSE {" );
	SetAseKey( m_pKeyword, ASEKEY::BITMAP,				L"*BITMAP " );

	SetAseKey( m_pKeyword, ASEKEY::GEOMOBJECT,			L"*GEOMOBJECT {" );
	SetAseKey( m_pKeyword, ASEKEY::NODE_NAME,			L"*NODE_NAME " );
	SetAseKey( m_pKeyword, ASEKEY::NODE_PARENT,			L"*NODE_PARENT " );
	SetAseKey( m_pKeyword, ASEKEY::NODE_TM,				L"*NODE_TM {" );
	SetAseKey( m_pKeyword, ASEKEY::TM_ROW0,				L"*TM_ROW0 " );
	SetAseKey( m_pKeyword, ASEKEY::TM_ROW1,				L"*TM_ROW1 " );
	SetAseKey( m_pKeyword, ASEKEY::TM_ROW2,				L"*TM_ROW2 " );
	SetAseKey( m_pKeyword, ASEKEY::TM_ROW3,				L"*TM_ROW3 " );
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
	SetAseKey( m_pKeyword, ASEKEY::TM_ANIMATION,		L"*TM_ANIMATION {" );
	SetAseKey( m_pKeyword, ASEKEY::CONTROL_POS_TRACK,	L"*CONTROL_POS_TRACK {" );
	SetAseKey( m_pKeyword, ASEKEY::CONTROL_SCALE_TRACK,	L"*CONTROL_SCALE_TRACK {" );
	SetAseKey( m_pKeyword, ASEKEY::CONTROL_ROT_TRACK,	L"*CONTROL_ROT_TRACK {" );
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

	if( m_pSceneData != NULL )
		delete m_pSceneData;
	if( m_pMaterialData != NULL )
		delete[] m_pMaterialData;
	if( m_pGeomObjectData != NULL )
		delete[] m_pGeomObjectData;

	m_pSceneData		=	NULL;
	m_pMaterialData		=	NULL;
	m_pGeomObjectData	=	NULL;

	//	Animation Test
	m_aniBase.iStartFrame		= 0;
	m_aniBase.iEndFrame			= 0;
	m_aniBase.dID				= 0;
	m_aniBase.bLoop				= TRUE;

	m_aniCurrent.iStartFrame	= 0;
	m_aniCurrent.iEndFrame		= 0;
	m_aniCurrent.dID			= 0;
	m_aniCurrent.bLoop			= FALSE;

	ANIMATIONMAP::iterator itE;
	for( itE = m_mapAnimation.begin() ; itE != m_mapAnimation.end() ; itE++ )
		delete itE->second;
	
	m_mapAnimation.clear();
	//	End
}

BOOL ASEParser::LoadFile( LPWSTR _FileName )
{
	Cleanup();

	//	파일명 확인
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
		
		BeginScene( fp, sLine );
		BeginMaterialList( fp, sLine );
		BeginGeomObject( fp, sLine );
	}
	
	//	test
	//CDebugConsole::GetInstance()->Message( L"Start TMProcessing\n" );
	TMProcessing();
	//	test
	//CDebugConsole::GetInstance()->Message( L"Start TFACEProcessing\n" );
	TFaceProcessing();
		
	fclose( fp );
	
	//	FrameMove test
	m_iCurrentFrame		= 0;
	m_dBeginTime		= timeGetTime();

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

VOID ASEParser::FrameMove()
{
	m_dCurrentTime	= timeGetTime();

	/*if( m_dCurrentTime > m_dBeginTime + m_pSceneData->iFrameSpeed )
	{
		m_iCurrentFrame++;

		m_dBeginTime = m_dCurrentTime;

		if( m_iCurrentFrame > m_pSceneData->iLastFrame )
			m_iCurrentFrame = m_pSceneData->iFirstFrame;
	}*/
	//	Animation Test
	if( m_dCurrentTime > m_dBeginTime + m_pSceneData->iFrameSpeed )
	{
		m_iCurrentFrame++;

		m_dBeginTime = m_dCurrentTime;

		if( m_iCurrentFrame > m_aniCurrent.iEndFrame )
		{
			if( !m_aniCurrent.bLoop )
			{
				m_aniCurrent.dID			= m_aniBase.dID;
				m_aniCurrent.iStartFrame	= m_aniBase.dID;
				m_aniCurrent.iEndFrame		= m_aniBase.iEndFrame;
				m_aniCurrent.bLoop			= m_aniCurrent.bLoop;
			}
			m_iCurrentFrame = m_aniCurrent.iStartFrame;
		}
	}
	//	End
}

VOID ASEParser::GetAniTrack( D3DXMATRIX& matOut, const INT _iIndex )
{	
	//	GetAniTrack
	LPGEOMOBJECTDATA	pGeo =	&m_pGeomObjectData[ _iIndex ];

	D3DXMATRIX			matA;
	D3DXMatrixIdentity( &matA );

	INT		iSizeR = pGeo->TrackRot.size();
	INT		iSizeP = pGeo->TrackPos.size();

	if( iSizeR && pGeo->TrackRot[ 0 ]->iFrame <= m_iCurrentFrame )
	{
		INT iIndex;

		if( 1 == iSizeR )
		{
			D3DXQUATERNION q;
			
			q.x = pGeo->TrackRot[ 0 ]->x;
			q.y = pGeo->TrackRot[ 0 ]->y;
			q.z = pGeo->TrackRot[ 0 ]->z;
			q.w = pGeo->TrackRot[ 0 ]->w;

			D3DXMatrixRotationQuaternion( &matA, &q );
		}
		else if( pGeo->TrackRot[ iSizeR - 1 ]->iFrame <= m_iCurrentFrame )
		{
			D3DXQUATERNION q;

			iIndex = iSizeR - 1;

			q.x = pGeo->TrackRot[ iIndex ]->x;
			q.y = pGeo->TrackRot[ iIndex ]->y;
			q.z = pGeo->TrackRot[ iIndex ]->z;
			q.w = pGeo->TrackRot[ iIndex ]->w;

			D3DXMatrixRotationQuaternion( &matA, &q );
		}
		else
		{
			for( INT i=0 ; i<iSizeR-1 ; i++ )
			{
				if( pGeo->TrackRot[ i ]->iFrame <= m_iCurrentFrame && m_iCurrentFrame < pGeo->TrackRot[ i + 1 ]->iFrame )
				{
					D3DXQUATERNION q;
					D3DXQUATERNION q1;
					D3DXQUATERNION q2;

					iIndex = i;

					q1.x = pGeo->TrackRot[ iIndex ]->x;
					q1.y = pGeo->TrackRot[ iIndex ]->y;
					q1.z = pGeo->TrackRot[ iIndex ]->z;
					q1.w = pGeo->TrackRot[ iIndex ]->w;

					q2.x = pGeo->TrackRot[ iIndex + 1 ]->x;
					q2.y = pGeo->TrackRot[ iIndex + 1 ]->y;
					q2.z = pGeo->TrackRot[ iIndex + 1 ]->z;
					q2.w = pGeo->TrackRot[ iIndex + 1 ]->w;

					FLOAT w = static_cast< FLOAT >( ( m_iCurrentFrame - pGeo->TrackRot[ i ]->iFrame ) / ( pGeo->TrackRot[ i + 1 ]->iFrame - pGeo->TrackRot[ i ]->iFrame ) );

					q = q1 + w * ( q2 - q1 );

					D3DXMatrixRotationQuaternion( &matA, &q );
					break;
				}
			}
		}
	}
	else
	{
		matA = pGeo->TmData.matLocal;
	}

	if( iSizeP && pGeo->TrackPos[ 0 ]->iFrame <= m_iCurrentFrame )
	{
		INT iIndex;

		if( 1 == iSizeP )
		{
			D3DXVECTOR3 p;
			
			p.x = pGeo->TrackPos[ 0 ]->x;
			p.y = pGeo->TrackPos[ 0 ]->y;
			p.z = pGeo->TrackPos[ 0 ]->z;

			matA._41 = p.x;
			matA._42 = p.y;
			matA._43 = p.z;
		}
		else if( pGeo->TrackPos[ iSizeP - 1 ]->iFrame <= m_iCurrentFrame )
		{
			D3DXVECTOR3 p;

			iIndex = iSizeP - 1;

			p.x = pGeo->TrackPos[ iIndex ]->x;
			p.y = pGeo->TrackPos[ iIndex ]->y;
			p.z = pGeo->TrackPos[ iIndex ]->z;

			matA._41 = p.x;
			matA._42 = p.y;
			matA._43 = p.z;
		}
		else
		{
			for( INT i=0 ; i<iSizeP-1 ; i++ )
			{
				if( pGeo->TrackPos[ i ]->iFrame <= m_iCurrentFrame && m_iCurrentFrame < pGeo->TrackPos[ i + 1 ]->iFrame )
				{
					D3DXVECTOR3 p;
					D3DXVECTOR3 p1;
					D3DXVECTOR3 p2;

					iIndex = i;

					p1.x = pGeo->TrackPos[ iIndex ]->x;
					p1.y = pGeo->TrackPos[ iIndex ]->y;
					p1.z = pGeo->TrackPos[ iIndex ]->z;

					p2.x = pGeo->TrackPos[ iIndex + 1 ]->x;
					p2.y = pGeo->TrackPos[ iIndex + 1 ]->y;
					p2.z = pGeo->TrackPos[ iIndex + 1 ]->z;

					FLOAT w = static_cast< FLOAT >( ( m_iCurrentFrame - pGeo->TrackPos[ i ]->iFrame ) / ( pGeo->TrackPos[ i + 1 ]->iFrame - pGeo->TrackPos[ i ]->iFrame ) );

					p = p1 + w * ( p2 - p1 );
					
					matA._41 = p.x;
					matA._42 = p.y;
					matA._43 = p.z;

					break;
				}
			}
		}
	}
	else
	{
		matA._41 = pGeo->TmData.matLocal._41;
		matA._42 = pGeo->TmData.matLocal._42;
		matA._43 = pGeo->TmData.matLocal._43;
	}

	pGeo->matLocal = matA;
	//	End	GetAniTrack

	D3DXMATRIX matParent;
	D3DXMatrixIdentity( &matParent );

	if( pGeo->pParent )
		matParent = pGeo->pParent->matWorld;

	pGeo->matWorld = pGeo->matLocal * matParent;

	matOut = pGeo->matWorld;

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

BOOL ASEParser::BeginScene( FILE* _fp, LPWSTR _sLine )
{
	if( 0 == wcsnicmp( _sLine, m_pKeyword[ ASEKEY::SCENE ].sKey, m_pKeyword[ ASEKEY::SCENE ].iKey ) )
	{
		m_pSceneData = new SCENEDATA;

		while( !feof( _fp ) )
		{
			ReadLineFromFile( _fp, _sLine );

			if( L'}' == _sLine[ 0 ] )
				break;
			
			GetSceneFirstFrame( _sLine );
			GetSceneLastFrame( _sLine );
			GetSceneFrameSpeed( _sLine );
			GetSceneTicksPerFrame( _sLine );
		}
		return TRUE;
	}
	return FALSE;
}

BOOL ASEParser::GetSceneFirstFrame( LPWSTR _sLine )
{
	if( 0 == wcsnicmp( _sLine, m_pKeyword[ ASEKEY::SCENE_FIRSTFRAME ].sKey, m_pKeyword[ ASEKEY::SCENE_FIRSTFRAME ].iKey ) )
	{
		INT i = 0;
		swscanf( _sLine, L"%*s %d", &i );

		m_pSceneData->iFirstFrame = i;
		//CDebugConsole::GetInstance()->Messagef( L"*SCENE_FIRSTFRAME %d\n", m_pSceneData->iFirstFrame );

		return TRUE;
	}

	return FALSE;
}
BOOL ASEParser::GetSceneLastFrame( LPWSTR _sLine )
{
	if( 0 == wcsnicmp( _sLine, m_pKeyword[ ASEKEY::SCENE_LASTFRAME ].sKey, m_pKeyword[ ASEKEY::SCENE_LASTFRAME ].iKey ) )
	{
		INT i = 0;
		swscanf( _sLine, L"%*s %d", &i );

		m_pSceneData->iLastFrame = i;
		//CDebugConsole::GetInstance()->Messagef( L"*SCENE_LASTFRAME %d\n", m_pSceneData->iLastFrame );

		return TRUE;
	}

	return FALSE;
}
BOOL ASEParser::GetSceneFrameSpeed( LPWSTR _sLine )
{
	if( 0 == wcsnicmp( _sLine, m_pKeyword[ ASEKEY::SCENE_FRAMESPEED ].sKey, m_pKeyword[ ASEKEY::SCENE_FRAMESPEED ].iKey ) )
	{
		INT i = 0;
		swscanf( _sLine, L"%*s %d", &i );

		m_pSceneData->iFrameSpeed = i;
		//CDebugConsole::GetInstance()->Messagef( L"*SCENE_FRAMESPEED %d\n", m_pSceneData->iFrameSpeed );

		return TRUE;
	}

	return FALSE;
}
BOOL ASEParser::GetSceneTicksPerFrame( LPWSTR _sLine )
{
	if( 0 == wcsnicmp( _sLine, m_pKeyword[ ASEKEY::SCENE_TICKSPERFRAME ].sKey, m_pKeyword[ ASEKEY::SCENE_TICKSPERFRAME ].iKey ) )
	{
		INT i = 0;
		swscanf( _sLine, L"%*s %d", &i );

		m_pSceneData->iTickPerFrame = i;
		//CDebugConsole::GetInstance()->Messagef( L"*SCENE_TICKSPERFRAME %d\n", m_pSceneData->iTickPerFrame );

		return TRUE;
	}

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
		////CDebugConsole::GetInstance()->Messagef( L"*MATERIAL_COUNT %d\n", m_iNumMaterial );

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
			GetNodeParentName( _sLine );

			BeginNodeTM( _fp, _sLine );
			
			BeginMesh( _fp, _sLine );

			GetMaterialRef( _sLine );

			BeginAnimation( _fp, _sLine );
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
BOOL ASEParser::GetNodeParentName( LPWSTR _sLine )
{
	if( 0 == wcsnicmp( _sLine, m_pKeyword[ ASEKEY::NODE_PARENT ].sKey, m_pKeyword[ ASEKEY::NODE_PARENT ].iKey ) )
	{
		TCHAR	Name[ 516 ];
		StrQuot( Name, _sLine );

		m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].pNodeParentName = new TCHAR[ 1024 ];

		lstrcpy( m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].pNodeParentName, Name );

		TCHAR	str[ 516 ];
		wsprintf( str, L"NODE_PARENT_NAME : %s\n", m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].pNodeParentName );
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

			GetRow0( _sLine );
			GetRow1( _sLine );
			GetRow2( _sLine );
			GetRow3( _sLine );
		}
		return TRUE;
	}

	return FALSE;
}
BOOL ASEParser::GetRow0( LPWSTR _sLine )
{
	if(  0 == wcsnicmp( _sLine, m_pKeyword[ ASEKEY::TM_ROW0 ].sKey, m_pKeyword[ ASEKEY::TM_ROW0 ].iKey ) )
	{
		FLOAT x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;

		swscanf( _sLine, L"%*s %f %f %f", &x, &y, &z );

		m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].TmData.matWorld._11 = x;
		m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].TmData.matWorld._12 = z;
		m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].TmData.matWorld._13 = y;
		m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].TmData.matWorld._14 = w;

		return TRUE;
	}

	return FALSE;
}

BOOL ASEParser::GetRow1( LPWSTR _sLine )
{
	if(  0 == wcsnicmp( _sLine, m_pKeyword[ ASEKEY::TM_ROW1 ].sKey, m_pKeyword[ ASEKEY::TM_ROW1 ].iKey ) )
	{
		FLOAT x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;

		swscanf( _sLine, L"%*s %f %f %f", &x, &y, &z );

		m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].TmData.matWorld._31 = x;
		m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].TmData.matWorld._32 = z;
		m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].TmData.matWorld._33 = y;
		m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].TmData.matWorld._34 = w;

		return TRUE;
	}

	return FALSE;
}

BOOL ASEParser::GetRow2( LPWSTR _sLine )
{
	if(  0 == wcsnicmp( _sLine, m_pKeyword[ ASEKEY::TM_ROW2 ].sKey, m_pKeyword[ ASEKEY::TM_ROW2 ].iKey ) )
	{
		FLOAT x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;

		swscanf( _sLine, L"%*s %f %f %f", &x, &y, &z );

		m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].TmData.matWorld._21 = x;
		m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].TmData.matWorld._22 = z;
		m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].TmData.matWorld._23 = y;
		m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].TmData.matWorld._24 = w;

		return TRUE;
	}

	return FALSE;
}

BOOL ASEParser::GetRow3( LPWSTR _sLine )
{
	if(  0 == wcsnicmp( _sLine, m_pKeyword[ ASEKEY::TM_ROW3 ].sKey, m_pKeyword[ ASEKEY::TM_ROW3 ].iKey ) )
	{
		FLOAT x = 0.0f, y = 0.0f, z = 0.0f, w = 1.0f;

		swscanf( _sLine, L"%*s %f %f %f", &x, &y, &z );

		m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].TmData.matWorld._41 = x;
		m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].TmData.matWorld._42 = z;
		m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].TmData.matWorld._43 = y;
		m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].TmData.matWorld._44 = w;

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

		////CDebugConsole::GetInstance()->Messagef( L"MESH_NUMVERTEX : %d\n", m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].iNumMeshVertex );
		
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

		////CDebugConsole::GetInstance()->Messagef( L"MESH_NUMINDEX : %d\n", m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].iNumMeshFace );

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

		////CDebugConsole::GetInstance()->Messagef( L"MESH_NUMTFACES : %d\n", m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].iNumMeshTFace );

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

BOOL ASEParser::BeginAnimation( FILE* _fp, LPWSTR _sLine )
{
	if( 0 == wcsnicmp( _sLine, m_pKeyword[ ASEKEY::TM_ANIMATION ].sKey, m_pKeyword[ ASEKEY::TM_ANIMATION ].iKey ) )
	{
		while( !feof( _fp ) )
		{
			ReadLineFromFile( _fp, _sLine );

			if( L'}' == _sLine[ 0 ] )
				break;

			BeginPosTrack( _fp, _sLine );
			BeginScaleTrack( _fp, _sLine );
			BeginRotTrack( _fp, _sLine );
		}
		return TRUE;
	}

	return FALSE;
}
BOOL ASEParser::BeginPosTrack( FILE* _fp, LPWSTR _sLine )
{
	if( 0 == wcsnicmp( _sLine, m_pKeyword[ ASEKEY::CONTROL_POS_TRACK ].sKey, m_pKeyword[ ASEKEY::CONTROL_POS_TRACK ].iKey ) )
	{
		while( !feof( _fp ) )
		{
			ReadLineFromFile( _fp, _sLine );

			if( L'}' == _sLine[ 0 ] )
				break;

			INT		iTrack;
			FLOAT	x = 0.0f, y = 0.0f, z = 0.0f;
			swscanf( _sLine, L"%*s %d %f %f %f", &iTrack, &x, &y, &z );

			iTrack /= m_pSceneData->iTickPerFrame;

			LPTRACKDATA	pTrackData = new TRACKDATA;
			
			pTrackData->iFrame	= iTrack;
			pTrackData->x		= x;
			pTrackData->y		= z;
			pTrackData->z		= y;
			pTrackData->w		= 0.0f;

			m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].TrackPos.push_back( pTrackData );

		}
		return TRUE;
	}

	return FALSE;
}
BOOL ASEParser::BeginScaleTrack( FILE* _fp, LPWSTR _sLine )
{
	if( 0 == wcsnicmp( _sLine, m_pKeyword[ ASEKEY::CONTROL_SCALE_TRACK ].sKey, m_pKeyword[ ASEKEY::CONTROL_SCALE_TRACK ].iKey ) )
	{
		while( !feof( _fp ) )
		{
			ReadLineFromFile( _fp, _sLine );

			if( L'}' == _sLine[ 0 ] )
				break;

			INT		iTrack;
			FLOAT	x = 0.0f, y = 0.0f, z = 0.0f;
			swscanf( _sLine, L"%*s %d %f %f %f", &iTrack, &x, &y, &z );

			iTrack /= m_pSceneData->iTickPerFrame;

			LPTRACKDATA	pTrackData = new TRACKDATA;
			
			pTrackData->iFrame	= iTrack;
			pTrackData->x		= x;
			pTrackData->y		= z;
			pTrackData->z		= y;
			pTrackData->w		= 0.0f;

			m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].TrackScale.push_back( pTrackData );
		}
		return TRUE;
	}

	return FALSE;
}
BOOL ASEParser::BeginRotTrack( FILE* _fp, LPWSTR _sLine )
{
	if( 0 == wcsnicmp( _sLine, m_pKeyword[ ASEKEY::CONTROL_ROT_TRACK ].sKey, m_pKeyword[ ASEKEY::CONTROL_ROT_TRACK ].iKey ) )
	{
		while( !feof( _fp ) )
		{
			ReadLineFromFile( _fp, _sLine );

			if( L'}' == _sLine[ 0 ] )
				break;

			INT		iTrack;
			FLOAT	x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;
			swscanf( _sLine, L"%*s %d %f %f %f %f", &iTrack, &x, &y, &z, &w );

			iTrack /= m_pSceneData->iTickPerFrame;

			D3DXQUATERNION	q1;

			q1.x = sinf( w / 2.0f ) * x;
			q1.y = sinf( w / 2.0f ) * z;
			q1.z = sinf( w / 2.0f ) * y;
			q1.w = cosf( w / 2.0f );

			INT iSize = m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].TrackRot.size();

			if( iSize == 0 )
			{
				LPTRACKDATA	pTrackData = new TRACKDATA;
			
				pTrackData->iFrame	= iTrack;
				pTrackData->x		= q1.x;
				pTrackData->y		= q1.y;
				pTrackData->z		= q1.z;
				pTrackData->w		= q1.w;

				m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].TrackRot.push_back( pTrackData );
			}
			else
			{
				D3DXQUATERNION	q2;
				D3DXQUATERNION	q3;

				LPTRACKDATA pPrevTrack = m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].TrackRot[ iSize - 1 ];

				q2.x = pPrevTrack->x;
				q2.y = pPrevTrack->y;
				q2.z = pPrevTrack->z;
				q2.w = pPrevTrack->w;

				D3DXQuaternionMultiply( &q3, &q2, &q1 );

				LPTRACKDATA	pTrackData = new TRACKDATA;
			
				pTrackData->iFrame	= iTrack;
				pTrackData->x		= q3.x;
				pTrackData->y		= q3.y;
				pTrackData->z		= q3.z;
				pTrackData->w		= q3.w;

				m_pGeomObjectData[ m_iCurrentGeomObjectIndex ].TrackRot.push_back( pTrackData );
			}
			
		}
		return TRUE;
	}

	return FALSE;
}

BOOL ASEParser::TMProcessing()
{
	//	Find Parernt
	for( INT i=0 ; i<m_iNumGeomObject ; i++ )
	{
		LPGEOMOBJECTDATA pGeoCurrent	= &m_pGeomObjectData[ i ];
		LPGEOMOBJECTDATA pGeoParent		= NULL;

		for( INT j=0 ; j<m_iNumGeomObject ; j++ )
		{
			LPGEOMOBJECTDATA pGeoTemp = &m_pGeomObjectData[ j ];

			if( pGeoCurrent->pNodeParentName == NULL )
				break;
			if( 0 == wcsicmp( pGeoCurrent->pNodeParentName , pGeoTemp->pNodeName ) )
			{
				pGeoParent				= pGeoTemp;
				pGeoCurrent->pParent	= pGeoTemp;
				break;
			}
		}

		if( pGeoParent )	//	부모가 있으면
		{
			D3DXMATRIX	matParent			= pGeoParent->TmData.matWorld;
			D3DXMATRIX	matParentInverse;
			D3DXMatrixInverse( &matParentInverse, NULL, &matParent );

			pGeoCurrent->TmData.matLocal = pGeoCurrent->TmData.matWorld * matParentInverse;
		}
		else
		{
			pGeoCurrent->TmData.matLocal = pGeoCurrent->TmData.matWorld;
		}
		
	}

	//	Vertex Inversion
	for( INT i=0 ; i<m_iNumGeomObject ; i++ )
	{
		LPGEOMOBJECTDATA pGeoCurrent = &m_pGeomObjectData[ i ];
		
		D3DXMATRIX matWorldInverse;
		D3DXMatrixInverse( &matWorldInverse, NULL, &pGeoCurrent->TmData.matWorld );

		for( INT j=0 ; j<pGeoCurrent->iNumMeshVertex ; j++ )
		{
			D3DXVECTOR3 vecPosition = pGeoCurrent->pMeshVertex[ j ].pos;

			D3DXVec3TransformCoord( &vecPosition, &vecPosition, &matWorldInverse );
	
			pGeoCurrent->pMeshVertex[ j ].pos = vecPosition;
		}
	}

	//	Setup Matrix
	for( INT i=0 ; i<m_iNumGeomObject ; i++ )
	{
		m_pGeomObjectData[ i ].matLocal = m_pGeomObjectData[ i ].TmData.matLocal;
		m_pGeomObjectData[ i ].matWorld = m_pGeomObjectData[ i ].TmData.matWorld;
	}
	return TRUE;
		
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

//	Animation Test
BOOL ASEParser::AddAnimationData( const DWORD _dType, DWORD _dID, INT _iStartFrame, INT _iEndFrame, BOOL _bLoop )
{
	//	aniBase
	if( ASEANI_IDLE == _dType )
	{
		m_aniBase.dID			= _dID;
		m_aniBase.iStartFrame	= _iStartFrame;
		m_aniBase.iEndFrame		= _iEndFrame;
		m_aniBase.bLoop			= TRUE;	//	강제!

		return TRUE;
	}

	//	aniCurrent
	if( ASEANI_POST_IDLE == _dType )
	{
		LPANIMATIONDATA pAnimationData = new ANIMATIONDATA;
		
		pAnimationData->dID			= _dID;
		pAnimationData->iStartFrame	= _iStartFrame;
		pAnimationData->iEndFrame	= _iEndFrame;
		pAnimationData->bLoop		= _bLoop;

		m_mapAnimation.insert( std::make_pair( _dID, pAnimationData ) );

		return TRUE;
	}

	return FALSE;
}

BOOL ASEParser::SetAnimation( DWORD _dID )
{
	ANIMATIONMAP::iterator itE = m_mapAnimation.find( _dID );;

	if( m_mapAnimation.end() == itE )
	{
		//CDebugConsole::GetInstance()->Message( L"SetAnimation() failed..." );
		return FALSE;
	}

	m_aniCurrent.dID			= itE->second->dID;
	m_aniCurrent.iStartFrame	= itE->second->iStartFrame;
	m_aniCurrent.iEndFrame		= itE->second->iEndFrame;
	m_aniCurrent.bLoop			= itE->second->bLoop;

	m_iCurrentFrame				= itE->second->iStartFrame;

	return TRUE;
}

//	End