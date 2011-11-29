#include "StdAfx.h"
#include "Light.h"

CLight::CLight(VOID)
{
	Clear();
}

CLight::~CLight(VOID)
{
	//Release();
}

HRESULT CLight::Create( LPDIRECT3DDEVICE9 a_pD3dDevice )
{
	m_pD3dDevice = a_pD3dDevice;

	return S_OK;
}

VOID CLight::Clear()
{
	ZeroMemory( &m_Mtrl, sizeof( D3DMATERIAL9 ) );
}

//HRESULT CLight::Release()
//{
//
//}

VOID CLight::DisableLight()
{
	m_pD3dDevice->LightEnable( 0, FALSE );
	m_pD3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	m_pD3dDevice->SetRenderState( D3DRS_AMBIENT, 0x00000000 );
}

VOID CLight::EnableLight()
{
	m_Mtrl.Diffuse.r = 1.0f;
	m_Mtrl.Diffuse.g = 1.0f;
	m_Mtrl.Diffuse.b = 1.0f;
	m_Mtrl.Diffuse.a = 1.0f;

	m_Mtrl.Ambient.r = 1.0f;
	m_Mtrl.Ambient.g = 1.0f;
	m_Mtrl.Ambient.b = 1.0f;
	m_Mtrl.Ambient.a = 1.0f;

	m_Mtrl.Emissive.r = 0.0f;
	m_Mtrl.Emissive.g = 0.0f;
	m_Mtrl.Emissive.b = 0.0f;
	m_Mtrl.Emissive.a = 0.0f;

	m_Mtrl.Power = 0.0f;

	m_Mtrl.Specular.r = 0.5f;
	m_Mtrl.Specular.g = 0.5f;
	m_Mtrl.Specular.b = 0.5f;
	m_Mtrl.Specular.a = 0.5f;


	m_pD3dDevice->SetMaterial( &m_Mtrl );
	//m_pD3dDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);

	D3DXVECTOR3 vecDir;
	D3DLIGHT9 light;
	ZeroMemory( &light, sizeof( D3DLIGHT9 ) );

	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 0.5f;
	light.Diffuse.g = 0.5f;
	light.Diffuse.b = 0.5f;
	light.Diffuse.a = 0.5f;

	light.Ambient.r = 0.5f;
	light.Ambient.g = 0.5f;
	light.Ambient.b = 0.5f;
	light.Ambient.a = 0.5f;

	light.Specular.r = 0.5f;
	light.Specular.g = 0.5f;
	light.Specular.b = 0.5f;
	light.Specular.a = 0.5f;

	light.Position.x = 0.0f;
	light.Position.y = 100.0f;
	light.Position.z = 0.0f;
	
	light.Range = 10000.0f;
	light.Theta = 0.0f;
	light.Phi = 0.0f;
	light.Falloff = 1.0f;
	light.Attenuation0 = 1.0f;

	//vecDir = D3DXVECTOR3( 1.0f, -1.0f, 0.0f );
	vecDir = D3DXVECTOR3( 0.0f, 0.0f, -1.0f );

	D3DXVec3Normalize( ( D3DXVECTOR3* )&light.Direction, &vecDir );
	
	m_pD3dDevice->SetLight( 0, &light );
	m_pD3dDevice->LightEnable( 0, TRUE );
	m_pD3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	m_pD3dDevice->SetRenderState( D3DRS_AMBIENT, 0xff000000 );
}
