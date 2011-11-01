#ifndef _CDEBUGINTERFACE_H_
#define _CDEBUGINTERFACE_H_

class CDebugInterface : public CSingleton<CDebugInterface>
{
public:
	CDebugInterface();
	~CDebugInterface();

	VOID Clear();
	HRESULT Create( LPDIRECT3DDEVICE9 a_pD3dDevice );
	HRESULT Release();

	VOID AddMessageFloat( CHAR* a_szLabel, FLOAT& a_fValue );
	VOID AddMessageVector( CHAR* a_szLabel, D3DXVECTOR3& a_vValue );

private:
	VOID CreateDebugInterface();

	TwBar *m_barDebug;
	INT	   m_iDebugBarCount;
	BOOL   m_bAddOK[255];

};

#endif