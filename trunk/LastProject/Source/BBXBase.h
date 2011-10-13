#pragma once

#ifndef _BBXBASE_H_
#define _BBXBASE_H_

class BBXBase
{
private:
	
public:
	typedef struct _BBXKEY
	{
		TCHAR	sKey[ 1024 ];	//	KeyWord
		INT		iKey;			//	Size

		enum {	BBXEXPORT = 0,
				BOUNDBOX_COUNT,
				BOUNDBOX_LIST,
				NAME,
				PIVOT,
				PLUS_SIZE,
				MINUS_SIZE,
				COLOR,
				END };
	}BBXKEY, *LPBBXKEY;
	
	typedef struct _VERTEX
	{
		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };

		D3DXVECTOR3	pos;
		D3DCOLOR	color;
	}VERTEX, *LPVERTEX;

	typedef struct _INDEX
	{
		DWORD	_0, _1;
	}INDEX, *LPINDEX;

	typedef struct _INFO
	{
		TCHAR		aName[ 1024 ];
		D3DXVECTOR3	vPivot;
		FLOAT		fPlusSize[ 3 ];
		FLOAT		fMinusSize[ 3 ];
		D3DCOLOR	Color;
	}INFO, *LPINFO;

	typedef struct _DATA
	{
		enum { PT = D3DPT_LINELIST };

		INFO		Info;
		VERTEX		aVertex[ 8 ];
		INDEX		aIndex[ 12 ];

		_DATA()
		{
			ZeroMemory( aVertex, sizeof( aVertex ) );
			ZeroMemory( aIndex, sizeof( aIndex ) );
		}
	}DATA, *LPDATA;

private:
	VOID		Initialize();
	VOID		Release();

public:
	BBXBase()
	{
		this->Initialize();
	}
	virtual ~BBXBase()
	{
		this->Release();
	}

	VOID		StrTrim( LPWSTR _sLine );
	VOID		StrQuot( LPWSTR pDst, LPCWSTR _sLine );

	VOID		ReadLineFromFile( FILE* _fp, LPWSTR _sLine )
	{
		fgetws( _sLine, 1024, _fp );
		StrTrim( _sLine );
	}

	VOID		SetBBXKey( LPBBXKEY _pBBXKey, DWORD _Type, LPWSTR _Str );

	VOID		SetVertex( LPDATA _pData );
	VOID		SetIndex( LPINDEX _pIndex );
};

#endif