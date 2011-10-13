#pragma once

#ifndef _ASEBASE_H_
#define _ASEBASE_H_

class ASEBase
{
private:

public:
	typedef struct _ASEKEY
	{
		TCHAR	sKey[ 1024 ];	//	Keyword
		INT		iKey;			//	Size

		enum {	ASCIIEXPORT = 0,
				MATERIAL_LIST,
				MATERIAL_COUNT,
				MATERIAL,
				MAP_DIFFUSE,
				BITMAP,
				GEOMOBJECT,
				NODE_NAME,
				NODE_TM,
				MESH,
				MESH_NUMVERTEX,
				MESH_NUMFACES,
				MESH_VERTEX_LIST,
				MESH_FACE_LIST,
				MESH_NUMTVERTEX,
				MESH_TVERTLIST,
				MESH_NUMTFACES,
				MESH_TFACELIST,
				MESH_NORMALS,
				MESH_VERTEXNORMAL,
				MATERIAL_REF,
				END };
	}ASEKEY, *LPASEKEY;

	typedef struct _VERTEX
	{
		enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 };

		D3DXVECTOR3	pos;
		D3DXVECTOR3 normal;
		D3DXVECTOR2 texcoord;
	}VERTEX, *LPVERTEX;

	typedef struct _INDEX
	{
		DWORD	_0, _1, _2;
	}INDEX, *LPINDEX;

	typedef struct _GEOMOBJECTDATA
	{
		LPWSTR		pNodeName;
		INT			iNumMeshVertex;
		INT			iNumMeshFace;
		LPVERTEX	pMeshVertex;
		LPINDEX		pMeshFace;

		INT			iNumMeshTVertex;
		INT			iNumMeshTFace;
		LPVERTEX	pMeshTVertex;
		LPINDEX		pMeshTFace;

		INT			iNumVertex;
		INT			iNumIndex;
		LPVERTEX	pVertex;
		LPINDEX		pIndex;

		INT			MaterialREF;

		_GEOMOBJECTDATA()
		{
			pNodeName		=	NULL;

			pMeshVertex		=	NULL;
			pMeshFace		=	NULL;
			pMeshTVertex	=	NULL;
			pMeshTFace		=	NULL;
			pVertex			=	NULL;
			pIndex			=	NULL;

			// test
			MaterialREF		=	0;
		}
		~_GEOMOBJECTDATA()
		{
			if( pNodeName != NULL )
				delete[] pNodeName;

			if( pMeshVertex != NULL )
				delete[] pMeshVertex;
			if( pMeshFace != NULL )
				delete[] pMeshFace;
			if( pMeshTVertex != NULL )
				delete[] pMeshTVertex;
			if( pMeshTFace != NULL )
				delete[] pMeshTFace;
			if( pVertex != NULL )
				delete[] pVertex;
			if( pIndex != NULL )
				delete[] pIndex;
		}
	}GEOMOBJECTDATA, *LPGEOMOBJECTDATA;

	typedef struct _MATERIALDATA
	{
		LPWSTR		pTextureFileName;

		_MATERIALDATA()
		{
			pTextureFileName = NULL;
		}
		~_MATERIALDATA()
		{
			if( pTextureFileName != NULL )
				delete[] pTextureFileName;
		}
	}MATERIALDATA, *LPMATERIALDATA;
private:
	VOID		Initialize();
	VOID		Release();
	
public:
	ASEBase()
	{
		this->Initialize();
	}
	~ASEBase()
	{
		this->Release();
	}

	VOID		StrTrim( LPWSTR _sLine );
	VOID		StrQuot( LPWSTR pDst, LPCWSTR pSrc );

	VOID		ReadLineFromFile( FILE* _fp, LPWSTR _sLine )
	{
		fgetws( _sLine, 1024, _fp );
		StrTrim( _sLine );
	}

	VOID		SetAseKey( LPASEKEY _pAseKey, DWORD _Type, LPWSTR _Str );

};

#endif