#pragma once

#ifndef _ASEBASE_H_
#define _ASEBASE_H_

#include <vector>
#include <map>

//	Animation Test
#define ASEANI_IDLE			1000
#define ASEANI_POST_IDLE	1001
//	End

class ASEBase
{
private:

public:
	typedef struct _ASEKEY
	{
		TCHAR	sKey[ 1024 ];	//	Keyword
		INT		iKey;			//	Size

		enum {	ASCIIEXPORT = 0,
				SCENE,
				SCENE_FIRSTFRAME,		//	*SCENE {
				SCENE_LASTFRAME,
				SCENE_FRAMESPEED,
				SCENE_TICKSPERFRAME,
				MATERIAL_LIST,			//	*MATERIAL_LIST {
				MATERIAL_COUNT,
				MATERIAL,
				MAP_DIFFUSE,
				BITMAP,
				GEOMOBJECT,				//	*GEOMOJECT {
				NODE_NAME,
				NODE_PARENT,
				NODE_TM,
				TM_ROW0,
				TM_ROW1,
				TM_ROW2,
				TM_ROW3,
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
				TM_ANIMATION,
				CONTROL_POS_TRACK,
				CONTROL_SCALE_TRACK,
				CONTROL_ROT_TRACK,
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

	typedef struct _TMDATA
	{
		D3DXMATRIX	matWorld;
		D3DXMATRIX	matLocal;

		FLOAT		Px, Py, Pz;
		FLOAT		Rx, Ry, Rz, Rw;
		FLOAT		Sx,	Sy, Sz;
	}TMDATA, *LPTMDATA;

	typedef struct _TRACKDATA
	{
		INT		iFrame;
		FLOAT	x, y, z, w;
	}TRACKDATA, *LPTRACKDATA;

	typedef std::vector< LPTRACKDATA > TRACKLIST;

	typedef struct _GEOMOBJECTDATA
	{
		_GEOMOBJECTDATA*	pParent;
		
		LPWSTR				pNodeName;
		LPWSTR				pNodeParentName;

		INT					iNumMeshVertex;
		INT					iNumMeshFace;
		LPVERTEX			pMeshVertex;
		LPINDEX				pMeshFace;

		INT					iNumMeshTVertex;
		INT					iNumMeshTFace;
		LPVERTEX			pMeshTVertex;
		LPINDEX				pMeshTFace;

		TMDATA				TmData;
		D3DXMATRIX			matLocal;
		D3DXMATRIX			matWorld;

		TRACKLIST			TrackPos;
		TRACKLIST			TrackScale;
		TRACKLIST			TrackRot;

		INT					iNumVertex;		//	TFaceProcessing()을 거친 정보
		INT					iNumIndex;
		LPVERTEX			pVertex;
		LPINDEX				pIndex;

		INT					MaterialREF;

		_GEOMOBJECTDATA()
		{
			pParent			=	NULL;

			pMeshVertex		=	NULL;
			pMeshFace		=	NULL;
			pMeshTVertex	=	NULL;
			pMeshTFace		=	NULL;
			pVertex			=	NULL;
			pIndex			=	NULL;

			pNodeName		=	NULL;
			pNodeParentName	=	NULL;

			MaterialREF		=	0;
		}
		virtual ~_GEOMOBJECTDATA()
		{
			SAFE_DELETE_ARRAY( pMeshVertex );
			SAFE_DELETE_ARRAY( pMeshFace );
			SAFE_DELETE_ARRAY( pMeshTVertex );
			SAFE_DELETE_ARRAY( pMeshTFace );
			SAFE_DELETE_ARRAY( pVertex );
			SAFE_DELETE_ARRAY( pIndex );

			SAFE_DELETE_ARRAY( pNodeName );
			SAFE_DELETE_ARRAY( pNodeParentName );

			TRACKLIST::iterator itE;
			for( itE = TrackPos.begin() ; itE != TrackPos.end() ; itE++ )
				delete (*itE);
			TrackPos.clear();

			for( itE = TrackScale.begin() ; itE != TrackScale.end() ; itE++ )
				delete (*itE);
			TrackScale.clear();

			for( itE = TrackRot.begin() ; itE != TrackRot.end() ; itE++ )
				delete (*itE);
			TrackRot.clear();

			MaterialREF		=	0;
		}
	}GEOMOBJECTDATA, *LPGEOMOBJECTDATA;

	typedef struct _MATERIALDATA
	{
		LPWSTR		pTextureFileName;

		_MATERIALDATA()
		{
			pTextureFileName = NULL;
		}
		virtual ~_MATERIALDATA()
		{
			SAFE_DELETE_ARRAY( pTextureFileName );
		}
	}MATERIALDATA, *LPMATERIALDATA;

	typedef struct _SCENEDATA
	{
		INT		iFirstFrame;
		INT		iLastFrame;
		INT		iFrameSpeed;
		INT		iTickPerFrame;
	}SCENEDATA, *LPSCENEDATA;

	typedef struct _ANIMATIONDATA
	{
		DWORD	dID;
		INT		iStartFrame;
		INT		iEndFrame;
		BOOL	bLoop;
	}ANIMATIONDATA, *LPANIMATIONDATA;

	typedef std::map< DWORD, LPANIMATIONDATA > ANIMATIONMAP;

private:
	VOID		Initialize();
	VOID		Release();
	
public:
	ASEBase()
	{
		this->Initialize();
	}
	virtual ~ASEBase()
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