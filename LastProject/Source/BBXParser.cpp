#include "stdafx.h"
#include "BBXParser.h"

VOID BBXParser::Initialize()
{
	m_pKeyword	= NULL;
	m_pData		= NULL;
	
	m_iNumBoundBox	=	0;
	m_iCurrentIndex	=	0;

	InitKeyword();
}

VOID BBXParser::InitKeyword()
{
	m_pKeyword = new BBXKEY[ BBXKEY::END ];

	SetBBXKey( m_pKeyword, BBXKEY::BBXEXPORT,		L"*Q_BREAK_BOUNDBOX" );
	SetBBXKey( m_pKeyword, BBXKEY::BOUNDBOX_COUNT,	L"*BOUNDBOX_COUNT" );
	SetBBXKey( m_pKeyword, BBXKEY::BOUNDBOX_LIST,	L"*BOUNDBOX_LIST {" );
	SetBBXKey( m_pKeyword, BBXKEY::NAME,			L"*NAME" );
	SetBBXKey( m_pKeyword, BBXKEY::PIVOT,			L"*PIVOT" );
	SetBBXKey( m_pKeyword, BBXKEY::PLUS_SIZE,		L"*PLUS_SIZE" );
	SetBBXKey( m_pKeyword, BBXKEY::MINUS_SIZE,		L"*MINUS_SIZE" );
	SetBBXKey( m_pKeyword, BBXKEY::COLOR,			L"*COLOR" );
}

VOID BBXParser::Release()
{
	if( m_pKeyword != NULL )
		delete[] m_pKeyword;

	Cleanup();
}

VOID BBXParser::Cleanup()
{
	m_iNumBoundBox	= 0;
	m_iCurrentIndex	= 0;

	if( m_pData != NULL )
		delete[] m_pData;
}

BOOL BBXParser::LoadFile( LPWSTR _FileName )
{
	Cleanup();

	//CDebugConsole::GetInstance()->Messagef( L"BBXParser\n" );
	////CDebugConsole::GetInstance()->Message( _FileName );
	//CDebugConsole::GetInstance()->Messagef( L"\n" );

	FILE*	fp;
	fp = _wfopen( _FileName, L"rt" );
	TCHAR	sLine[ 1024 ];

	ReadLineFromFile( fp, sLine );
	if( !CheckFile( fp, sLine ) )
		return FALSE;

	while( !feof( fp ) )
	{
		ReadLineFromFile( fp, sLine );

		GetNumBoundBox( sLine );

		BeginBoundBoxList( fp, sLine );
	}

	//	렌더링 될 충돌박스를 만든다
	CreateCube( m_iNumBoundBox, m_pData );

	fclose( fp );

	return TRUE;
}

BOOL BBXParser::GetData( INT _Index, BBXParser::DATA& _Out )
{
	lstrcpy( _Out.Info.aName, m_pData[ _Index ].Info.aName );
	_Out.Info.vPivot = m_pData[ _Index ].Info.vPivot;
	memcpy( _Out.Info.fPlusSize, m_pData[ _Index ].Info.fPlusSize, sizeof( _Out.Info.fPlusSize ) );
	memcpy( _Out.Info.fMinusSize, m_pData[ _Index ].Info.fMinusSize, sizeof( _Out.Info.fMinusSize ) );
	_Out.Info.Color = m_pData[ _Index ].Info.Color;

	memcpy( _Out.aVertex, m_pData[ _Index ].aVertex, sizeof( _Out.aVertex ) );
	memcpy( _Out.aIndex, m_pData[ _Index ].aIndex, sizeof( _Out.aIndex ) );

	return TRUE;
}

BOOL BBXParser::CheckFile( FILE *_fp, LPWSTR _sLine )
{
	if( 0 == wcsnicmp( _sLine, m_pKeyword[ BBXKEY::BBXEXPORT ].sKey, m_pKeyword[ BBXKEY::BBXEXPORT ].iKey ) )
	{
		//CDebugConsole::GetInstance()->Messagef( L"CHECK FILE : SUCCESS\n" );
		return TRUE;
	}

	//CDebugConsole::GetInstance()->Messagef( L"CHECK FILE : FAIL\n" );

	MessageBox( NULL, L"BBX FILE이 아닙니다", NULL, MB_OK );

	return FALSE;
}

BOOL BBXParser::GetNumBoundBox( LPWSTR _sLine )
{
	if( 0 == wcsnicmp( _sLine, m_pKeyword[ BBXKEY::BOUNDBOX_COUNT ].sKey, m_pKeyword[ BBXKEY::BOUNDBOX_COUNT ].iKey ) )
	{
		INT iCount = 0;
		swscanf( _sLine, L"%*s %d", &iCount );

		m_iNumBoundBox = iCount;
		//CDebugConsole::GetInstance()->Messagef( L"*BOUNDBOX_COUNT %d\n", m_iNumBoundBox );

		m_pData = new DATA[ m_iNumBoundBox ];

		return TRUE;
	}

	return FALSE;
}

BOOL BBXParser::BeginBoundBoxList( FILE* _fp, LPWSTR _sLine )
{
	if( 0 == wcsnicmp( _sLine, m_pKeyword[ BBXKEY::BOUNDBOX_LIST ].sKey, m_pKeyword[ BBXKEY::BOUNDBOX_LIST ].iKey ) )
	{
		while( !feof( _fp ) )
		{
			ReadLineFromFile( _fp, _sLine );

			if( L'}' == _sLine[ 0 ] )
				break;

			GetBoundBoxName( _sLine );
			GetBoundBoxPivot( _sLine );
			GetBoundBoxPlusSize( _sLine );
			GetBoundBoxMinusSize( _sLine );
			GetBoundBoxColor( _sLine );
		}
		
		m_iCurrentIndex++;
		return TRUE;
	}

	return FALSE;
}

BOOL BBXParser::GetBoundBoxName( LPWSTR _sLine )
{
	if( 0 == wcsnicmp( _sLine, m_pKeyword[ BBXKEY::NAME ].sKey, m_pKeyword[ BBXKEY::NAME ].iKey ) )
	{
		TCHAR Name[ 1024 ];
		StrQuot( Name, _sLine );

		lstrcpy( m_pData[ m_iCurrentIndex ].Info.aName, Name );

		TCHAR str[ 1024 ];
		wsprintf( str, L"*NAME : %s\n", Name );
		////CDebugConsole::GetInstance()->Message( str );

		return TRUE;
	}

	return FALSE;
}

BOOL BBXParser::GetBoundBoxPivot( LPWSTR _sLine )
{
	if( 0 == wcsnicmp( _sLine, m_pKeyword[ BBXKEY::PIVOT ].sKey, m_pKeyword[ BBXKEY::PIVOT ].iKey ) )
	{
		FLOAT x = 0.0f, y = 0.0f, z = 0.0f;
		swscanf( _sLine, L"%*s %f %f %f", &x, &y, &z );

		m_pData[ m_iCurrentIndex ].Info.vPivot.x = x;
		m_pData[ m_iCurrentIndex ].Info.vPivot.y = y;
		m_pData[ m_iCurrentIndex ].Info.vPivot.z = z;

		//CDebugConsole::GetInstance()->Messagef( L"*PIVOT %f %f %f\n", x, y, z );

		return TRUE;
	}

	return FALSE;
}

BOOL BBXParser::GetBoundBoxPlusSize( LPWSTR _sLine )
{
	if( 0 == wcsnicmp( _sLine, m_pKeyword[ BBXKEY::PLUS_SIZE ].sKey, m_pKeyword[ BBXKEY::PLUS_SIZE ].iKey ) )
	{
		FLOAT x = 0.0f, y = 0.0f, z = 0.0f;
		swscanf( _sLine, L"%*s %f %f %f", &x, &y, &z );

		m_pData[ m_iCurrentIndex ].Info.fPlusSize[ 0 ] = x;
		m_pData[ m_iCurrentIndex ].Info.fPlusSize[ 1 ] = y;
		m_pData[ m_iCurrentIndex ].Info.fPlusSize[ 2 ] = z;

		//CDebugConsole::GetInstance()->Messagef( L"*PLUS_SIZE %f %f %f\n", x, y, z );

		return TRUE;
	}

	return FALSE;
}

BOOL BBXParser::GetBoundBoxMinusSize( LPWSTR _sLine )
{
	if( 0 == wcsnicmp( _sLine, m_pKeyword[ BBXKEY::MINUS_SIZE ].sKey, m_pKeyword[ BBXKEY::MINUS_SIZE ].iKey ) )
	{
		FLOAT x = 0.0f, y = 0.0f, z = 0.0f;
		swscanf( _sLine, L"%*s %f %f %f", &x, &y, &z );

		m_pData[ m_iCurrentIndex ].Info.fMinusSize[ 0 ] = x;
		m_pData[ m_iCurrentIndex ].Info.fMinusSize[ 1 ] = y;
		m_pData[ m_iCurrentIndex ].Info.fMinusSize[ 2 ] = z;

		//CDebugConsole::GetInstance()->Messagef( L"*MINUS_SIZE %f %f %f\n", x, y, z );

		return TRUE;
	}

	return FALSE;
}

BOOL BBXParser::GetBoundBoxColor( LPWSTR _sLine )
{
	if( 0 == wcsnicmp( _sLine, m_pKeyword[ BBXKEY::COLOR ].sKey, m_pKeyword[ BBXKEY::COLOR ].iKey ) )
	{
		DWORD	Color = 0;
		swscanf( _sLine, L"%*s %x", &Color );

		m_pData[ m_iCurrentIndex ].Info.Color = Color;
		
		//CDebugConsole::GetInstance()->Messagef( L"*COLOR %X\n", Color );

		return TRUE;
	}

	return FALSE;
}

VOID BBXParser::CreateCube( INT _iNumBoundBox, LPDATA _pData )
{
	for( INT i=0 ; i<_iNumBoundBox ; i++ )
	{
		D3DXVECTOR3 vPivot = _pData[ i ].Info.vPivot;

		SetVertex( &_pData[ i ] );
		/*SetVertex(	&m_pData[ i ].aVertex[ 0 ], 
					vPivot.x + m_pData[ i ].Info.fMinusSize[ 0 ],
					vPivot.y + m_pData[ i ].Info.fPlusSize[ 1 ],
					vPivot.z + m_pData[ i ].Info.fPlusSize[ 2 ],
					m_pData[ i ].Info.Color );
		SetVertex(	&m_pData[ i ].aVertex[ 1 ], 
					vPivot.x + m_pData[ i ].Info.fPlusSize[ 0 ],
					vPivot.y + m_pData[ i ].Info.fPlusSize[ 1 ],
					vPivot.z + m_pData[ i ].Info.fPlusSize[ 2 ],
					m_pData[ i ].Info.Color );
		SetVertex(	&m_pData[ i ].aVertex[ 2 ], 
					vPivot.x + m_pData[ i ].Info.fPlusSize[ 0 ],
					vPivot.y + m_pData[ i ].Info.fPlusSize[ 1 ],
					vPivot.z + m_pData[ i ].Info.fMinusSize[ 2 ],
					m_pData[ i ].Info.Color );
		SetVertex(	&m_pData[ i ].aVertex[ 3 ], 
					vPivot.x + m_pData[ i ].Info.fMinusSize[ 0 ],
					vPivot.y + m_pData[ i ].Info.fPlusSize[ 1 ],
					vPivot.z + m_pData[ i ].Info.fMinusSize[ 2 ],
					m_pData[ i ].Info.Color );
		SetVertex(	&m_pData[ i ].aVertex[ 4 ], 
					vPivot.x + m_pData[ i ].Info.fMinusSize[ 0 ],
					vPivot.y + m_pData[ i ].Info.fMinusSize[ 1 ],
					vPivot.z + m_pData[ i ].Info.fPlusSize[ 2 ],
					m_pData[ i ].Info.Color );
		SetVertex(	&m_pData[ i ].aVertex[ 5 ], 
					vPivot.x + m_pData[ i ].Info.fPlusSize[ 0 ],
					vPivot.y + m_pData[ i ].Info.fMinusSize[ 1 ],
					vPivot.z + m_pData[ i ].Info.fPlusSize[ 2 ],
					m_pData[ i ].Info.Color );
		SetVertex(	&m_pData[ i ].aVertex[ 6 ], 
					vPivot.x + m_pData[ i ].Info.fPlusSize[ 0 ],
					vPivot.y + m_pData[ i ].Info.fMinusSize[ 1 ],
					vPivot.z + m_pData[ i ].Info.fMinusSize[ 2 ],
					m_pData[ i ].Info.Color );
		SetVertex(	&m_pData[ i ].aVertex[ 7 ], 
					vPivot.x + m_pData[ i ].Info.fMinusSize[ 0 ],
					vPivot.y + m_pData[ i ].Info.fMinusSize[ 1 ],
					vPivot.z + m_pData[ i ].Info.fMinusSize[ 2 ],
					m_pData[ i ].Info.Color );*/

		SetIndex( _pData[ i ].aIndex );
	}
}
		
	