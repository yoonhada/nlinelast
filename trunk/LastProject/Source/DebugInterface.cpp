#include "Stdafx.h"
#include "DebugInterface.h"

CDebugInterface::CDebugInterface()
{

}

CDebugInterface::~CDebugInterface()
{

}

VOID CDebugInterface::Clear()
{
	ZeroMemory( m_bAddOK, sizeof(m_bAddOK) );
	m_iDebugBarCount = 0;
}

HRESULT CDebugInterface::Create( LPDIRECT3DDEVICE9 a_pD3dDevice )
{
	if( !TwInit( TW_DIRECT3D9, a_pD3dDevice ) )
	{
		MessageBox(CWinBase::GetInstance()->Get_hWnd(), L"Error", L"Cannot initialize AntTweakBar", MB_OK|MB_ICONERROR);
		//a_pD3dDevice->Release();
		//a_pD3dDevice->Get_pD3d()->Release();
		return E_FAIL;
	}

	CreateDebugInterface();
	
	return S_OK;
}

HRESULT CDebugInterface::Release()
{
	TwTerminate();

	return S_OK;
}

VOID CDebugInterface::CreateDebugInterface()
{
	m_barDebug = TwNewBar("Debug");
	TwDefine(" Debug color='128 128 128' alpha=155 text=light contained=true size='240 500' position='0 0' refresh=0.01 ");
}

VOID CDebugInterface::AddMessageFloat( CHAR* a_szLabel, FLOAT& a_fValue )
{
	CHAR szTemp[255];
	CHAR szTempLabel[255];
	sprintf( szTemp, "%s", a_szLabel  );
	sprintf( szTempLabel, " label='%s' ", a_szLabel  );
	
	CHAR szGetLabel[255];
	TwGetParam(m_barDebug, szTempLabel, "label", TW_PARAM_CSTRING, 255, szGetLabel );
	
	//이미 있는 Var인지 체크 하기
	if ( strcmpi( szTemp, szGetLabel ) != 0 )
	{				
		TwAddVarRO( m_barDebug, szTempLabel, TW_TYPE_FLOAT, &a_fValue, szTempLabel );
	}
}

VOID CDebugInterface::AddMessageVector( CHAR* a_szLabel, D3DXVECTOR3& a_vValue )
{
	CHAR szTemp[255];
	CHAR szTempLabel[255];
	CHAR szGetLabel[255];

	for(INT Loop=0; Loop<3; ++Loop)
	{
		sprintf( szTemp, "%s:%d", a_szLabel, Loop  );
		sprintf( szTempLabel, " label='%s:%d' ", a_szLabel, Loop  );
		
		TwGetParam(m_barDebug, szTempLabel, "label", TW_PARAM_CSTRING, 255, szGetLabel );

		//이미 있는 Var인지 체크 하기
		if ( strcmpi( szTemp, szGetLabel ) != 0 )
		{				
			switch(Loop)
			{
			case 0:
				TwAddVarRO( m_barDebug, szTempLabel, TW_TYPE_FLOAT, &a_vValue.x, szTempLabel );
				break;
			case 1:
				TwAddVarRO( m_barDebug, szTempLabel, TW_TYPE_FLOAT, &a_vValue.y, szTempLabel );
				break;
			case 2:
				TwAddVarRO( m_barDebug, szTempLabel, TW_TYPE_FLOAT, &a_vValue.z, szTempLabel );
				break;
			}
		}
	}
	
}