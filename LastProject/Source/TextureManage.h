/**
@file	TextureManage.h
@date	2011/09/19
@author	
@brief	TextureManage 인터페이스
*/

#ifndef _TextureManage_H_
#define _TextureManage_H_

#include "Manage.h"

class CTextureManage : public CSingleton<CTextureManage>, public IManage<LPDIRECT3DTEXTURE9>
{
	friend class CSingleton<CTextureManage>;
public:
	CTextureManage( void );
	virtual ~CTextureManage( void );

	BOOL Create();
	VOID Release();
};

#endif