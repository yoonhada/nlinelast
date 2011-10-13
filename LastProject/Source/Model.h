/**
@file	CManageCube.h
@date	2011/09/05
@author	
@brief	ť�� ����Ʈ
*/

#pragma once

class CCharCube;
class CQube;

class CModel : public virtual IObject
{
private:
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	LPDIRECT3DVERTEXBUFFER9 m_pTotalVB;   ///< ť�� ���� ���ؽ�
	LPDIRECT3DINDEXBUFFER9  m_pTotalIB;   ///< ť�� �ε���

	//std::vector<CCube*> m_vectorCube;
	std::vector<CQube*> m_vectorQube;
	//std::vector <CCube*>::iterator m_iterCube;
	std::vector <CQube*>::iterator m_iterQube;

	//VOID CreateCube();
	VOID CreateQube();

	INT m_iVectorNum;
	D3DXMATRIXA16 m_matMultMatrix;

public:
	enum { CUBE, QUBE, TEST };
	CModel( LPDIRECT3DDEVICE9 _pd3dDevice );
	virtual ~CModel();

	virtual VOID Clear();
	virtual HRESULT Create();
	virtual HRESULT Release();
	virtual VOID Render();
	virtual VOID Update();

	VOID CreateRandom( CCharCube* a_pCube, INT a_iFrameNum, const D3DXMATRIXA16& a_matChara );
};