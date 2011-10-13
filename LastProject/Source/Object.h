/**
	@file	I_Object.h
	@date	2011/09/17
	@author	�����
	@brief	������Ʈ �������̽�
*/

#ifndef _OBJECT_H_
#define _OBJECT_H_

/**
	@class	IObject
	@date	2011/09/17
	@author	�����
	@brief	������Ʈ �������̽�
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