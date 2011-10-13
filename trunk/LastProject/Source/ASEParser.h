#pragma once 

#ifndef _ASEPARSER_H_
#define _ASEPARSER_H_

#include "ASEBase.h"

class ASEParser : public ASEBase
{
private:
	VOID	Initialize();
	VOID	InitKeyword();
	VOID	Release();
	VOID	Cleanup();

	BOOL		CheckFile( FILE* _fp, LPWSTR _sLine );
	BOOL		CountGeometry( FILE* _fp );
	
	BOOL		BeginMaterialList( FILE* _fp, LPWSTR _sLine );
	BOOL		GetMaterialCount( LPWSTR _sLine );
	BOOL		BeginMaterial( FILE* _fp, LPWSTR _sLine );
	BOOL		BeginMapDiffuse( FILE* _fp, LPWSTR _sLine );
	BOOL		GetBitmap( LPWSTR _sLine );

	BOOL		BeginGeomObject( FILE* _fp, LPWSTR _sLine );
	BOOL		GetNodeName( LPWSTR _sLine );
	BOOL		BeginNodeTM( FILE* _fp, LPWSTR _sLine );
	BOOL		BeginMesh( FILE* _fp, LPWSTR _sLine );
	BOOL		GetMeshNumVertex( LPWSTR _sLine );
	BOOL		GetMeshNumFaces( LPWSTR _sLine );
	BOOL		BeginMeshVertexList( FILE* _fp, LPWSTR _sLine );
	BOOL		BeginMeshFaceList( FILE* _fp, LPWSTR _sLine );
	BOOL		GetMeshNumTVertex( LPWSTR _sLine );
	BOOL		BeginMeshTVertexList( FILE* _fp, LPWSTR _sLine );
	BOOL		GetMeshNumTFaces( LPWSTR _sLine );
	BOOL		BeginMeshTFaceList( FILE* _fp, LPWSTR _sLine );
	BOOL		BeginMeshNormals( FILE* _fp, LPWSTR _sLine );
	BOOL		GetMeshVertexNormal( LPWSTR _sLine );
	BOOL		GetMaterialRef( LPWSTR _sLine );

	BOOL		TFaceProcessing();

public:
	ASEParser()
	{
		this->Initialize();
	}
	~ASEParser()
	{
		this->Release();
	}
	
	BOOL		LoadFile( LPWSTR _FileName );
	BOOL		GetObjectData( const INT _Index, MATERIALDATA& _MaterialData, GEOMOBJECTDATA& _GeomObjectData );
	INT			GetNumObject();
	
private:
	LPASEKEY			m_pKeyword;
	LPMATERIALDATA		m_pMaterialData;
	LPGEOMOBJECTDATA	m_pGeomObjectData;

	INT					m_iNumMaterial;
	INT					m_iNumGeomObject;

	INT					m_iCurrentGeomObjectIndex;
	INT					m_iCurrentMaterialIndex;

public:

};

#endif