#include "Stdafx.h"
#include "ObjectManage.h"

CObjectManage::CObjectManage()
{
	Clear();
}

CObjectManage::~CObjectManage()
{
	Release();
}

VOID CObjectManage::Clear()
{

}

HRESULT CObjectManage::Create()
{
	
	return S_OK;
}

HRESULT CObjectManage::Release()
{

	return S_OK;
}