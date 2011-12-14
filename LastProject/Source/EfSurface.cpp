#include "Stdafx.h"
#include "EfSurface.h"
#include "Charactor.h"
#include "ASEViewer.h"

CEfSurface::CEfSurface()
{
	m_pDev	= NULL;

	m_pTx	=NULL;
	m_pTxSf	=NULL;
	m_pTxRs	=NULL;

	m_iTxW	= 1024;
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

	if(FAILED(m_pDev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackSurface)))
		return-1;

	pBackSurface->GetDesc( &dsc );
	pBackSurface->Release();

	FLOAT sx = FLOAT(dsc.Width);
	FLOAT sy = FLOAT(dsc.Height);

	m_pVtx[0] =	VtxwDUV(   0.f,	  0.f,	0.f, 0.4f, 0.4f);
	m_pVtx[1] =	VtxwDUV( 200.f,	  0.f,	0.f, 0.6f, 0.4f); 
	m_pVtx[2] =	VtxwDUV(   0.f,	200.f,	0.f, 0.4f, 0.6f); 
	m_pVtx[3] =	VtxwDUV( 200.f,	200.f,	0.f, 0.6f, 0.6f);

	return 0;
}

void CEfSurface::Destroy()
{
	SAFE_RELEASE(	m_pTxSf	);
	SAFE_RELEASE(	m_pTx	);
	SAFE_RELEASE(	m_pTxRs	);
}



INT CEfSurface::Restore()
{
	IDirect3DSurface9*	pSrfc=NULL;
	D3DSURFACE_DESC		dscColor;
	D3DSURFACE_DESC		dscDepth;

	if(FAILED(m_pDev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pSrfc)))
		return-1;

	pSrfc->GetDesc( &dscColor);
	pSrfc->Release();

	if(FAILED(m_pDev->GetDepthStencilSurface(&pSrfc)))
		return -1;

	pSrfc->GetDesc( &dscDepth);
	pSrfc->Release();



	D3DXCreateTexture(m_pDev, m_iTxW, m_iTxW, 1, D3DUSAGE_RENDERTARGET, dscColor.Format,  D3DPOOL_DEFAULT, &m_pTx);
	m_pTx->GetSurfaceLevel(0,&m_pTxSf);

	D3DSURFACE_DESC		dscTex;
	m_pTxSf->GetDesc( &dscTex);


	D3DXCreateRenderToSurface(m_pDev
		, dscTex.Width
		, dscTex.Height
		, dscColor.Format
		, TRUE
		, dscDepth.Format, &m_pTxRs);



	return 0;
}

INT CEfSurface::Update( IObject * a_pObject )
{
	HRESULT	hr=0;

	m_pDev->SetRenderState( D3DRS_LIGHTING, TRUE );
	m_pDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
	m_pDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );

	m_pDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	m_pDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDev->SetTextureStageState(0,  D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);


	hr = m_pTxRs->BeginScene(m_pTxSf, NULL);
	hr = m_pDev->Clear( 0L, NULL
		, D3DCLEAR_TARGET | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER
		, 0xFF006699, 1.0f, 0L );

	

	// Scene render
	a_pObject->Render();

	hr = m_pTxRs->EndScene(D3DX_FILTER_NONE);
	m_pDev->SetRenderState( D3DRS_LIGHTING, FALSE );
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


