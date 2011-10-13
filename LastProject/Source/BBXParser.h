#pragma once

#ifndef _BBXPARSER_H_
#define _BBXPARSER_H_

#include "BBXBase.h"

class BBXParser : public BBXBase
{
private:
	VOID		Initialize();
	VOID		InitKeyword();
	VOID		Release();
	VOID		Cleanup();

	BOOL		CheckFile( FILE* _fp, LPWSTR _sLine );
	BOOL		GetNumBoundBox( LPWSTR _sLine );
	BOOL		BeginBoundBoxList( FILE* _fp, LPWSTR _sLine );
	BOOL		GetBoundBoxName( LPWSTR _sLine );
	BOOL		GetBoundBoxPivot( LPWSTR _sLine );
	BOOL		GetBoundBoxPlusSize( LPWSTR _sLine );
	BOOL		GetBoundBoxMinusSize( LPWSTR _sLine );
	BOOL		GetBoundBoxColor( LPWSTR _sLine );

	VOID		CreateCube( INT _iNumBoundBox, LPDATA _pData );

public:
	BBXParser()
	{
		this->Initialize();
	}
	virtual ~BBXParser()
	{
		this->Release();
	}

	BOOL		LoadFile( LPWSTR _FileName );
	INT			GetNumBoundBox(){ return m_iNumBoundBox; }
	BOOL		GetData( INT _Index, BBXParser::DATA& _Out );

private:
	LPBBXKEY	m_pKeyword;
	LPDATA		m_pData;

	INT			m_iNumBoundBox;

	INT			m_iCurrentIndex;

public:

};

#endif