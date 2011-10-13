/**
	@file	I_Object.h
	@date	2011/09/17
	@author	백경훈
	@brief	오브젝트 인터페이스
*/

#ifndef _OBJECT_H_
#define _OBJECT_H_

/**
	@class	IObject
	@date	2011/09/17
	@author	백경훈
	@brief	오브젝트 인터페이스
*/

class IObject
{
public:
	IObject()			{ }
	virtual ~IObject()	{ }

	virtual VOID	Clear() = 0;
	virtual HRESULT Create() = 0;
	virtual HRESULT	Release() = 0;
	virtual VOID	Update() = 0;
	virtual VOID	Render() = 0;
};

#endif