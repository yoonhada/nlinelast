#include "Stdafx.h"
#include "EfSurface.h"
#include "Charactor.h"
#include "ASEViewer.h"

CEfSurface::CEfSurface()
{
	m_pDev	= NULL;

	m_pTx	=NULL;
	m_pSf	=NULL;

	m_iTxW	= 200;
}


CEfSurface::~CEfSurface()
{
	Destroy();
}


INT CEfSurface::Create(LPDIRECT3DDEVICE9 pDev)
{
	m_pDev	= pDev;

	//화면 크기 얻기

	IDirect3DSurface9*	pBackSurface=NULL;
	D3DSURFACE_DESC		dsc;

	if( FAILED ( m_pDev->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackSurface ) ) )
		return-1;
		
	pBackSurface->GetDesc( &dsc );
	pBackSurface->Release();

	FLOAT sx = FLOAT(dsc.Width);
	FLOAT sy = FLOAT(dsc.Height);

	m_pVtx[0] =	VtxwDUV(    200.f,	    200.f,	0.f, 0.0005f,0.05f, 0xFFFFFF);
	m_pVtx[1] =	VtxwDUV( sx-200.f,	    200.f,	0.f, 0.9995f,0.05f, 0xFFFFFF); 
	m_pVtx[2] =	VtxwDUV(    200.f,	 sy-200.f,	0.f, 0.0005f,0.95f, 0xFFFFFF); 
	m_pVtx[3] =	VtxwDUV( sx-200.f,	 sy-200.f,	0.f, 0.9995f,0.95f, 0xFFFFFF);

	return 0;
}

void CEfSurface::Destroy()
{
}



INT CEfSurface::Restore()
{
	m_pDev->CreateTexture(	m_iTxW, m_iTxW, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTx, NULL);
	m_pTx->GetSurfaceLevel(0,&m_pSf);

	return 0;
}


void CEfSurface::Invalidate()
{
	SAFE_RELEASE( m_pSf	);
	SAFE_RELEASE( m_pTx	);
}


INT CEfSurface::Update( ASEViewer * a_pAseViewer )
{
	HRESULT	hr=0;

	m_pDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
	m_pDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );

	m_pDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	m_pDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDev->SetTextureStageState(0,  D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	LPDIRECT3DSURFACE9	pSfOrgD = NULL;														// Back buffer Depth and stencil
	LPDIRECT3DSURFACE9	pSfOrgT = NULL;														// Back buffer target

	hr = m_pDev->GetRenderTarget(0, &pSfOrgT);
	hr = m_pDev->GetDepthStencilSurface(&pSfOrgD);
	hr = m_pDev->SetRenderTarget(0,m_pSf);

	hr = m_pDev->Clear( 0L, NULL, D3DCLEAR_TARGET| D3DCLEAR_ZBUFFER, 0xFFFF0000, 1.0f, 0L );

	// Scene render
	a_pAseViewer->Render();
	//CObjectManage::GetInstance()->Get_Charactor()[CObjectManage::GetInstance()->Get_CharTable( 0 )].Render();
	
	
	hr = m_pDev->SetRenderTarget(0,pSfOrgT);										//렌더 타겟을 원래 데로.
	hr = m_pDev->SetDepthStencilSurface(pSfOrgD);
	
	SAFE_RELEASE( pSfOrgT );
	SAFE_RELEASE( pSfOrgD );

	return 0;
}


void CEfSurface::Render()
{
	m_pDev->SetRenderState( D3DRS_ZENABLE,   FALSE);

	m_pDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
	m_pDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );

	m_pDev->SetRenderState( D3DRS_ALPHABLENDENABLE,   FALSE);

	m_pDev->SetTexture(0,m_pTx);
	m_pDev->SetFVF( VtxwDUV::FVF );
	m_pDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,m_pVtx,sizeof(VtxwDUV));

	m_pDev->SetRenderState( D3DRS_ZENABLE,   TRUE);
}


