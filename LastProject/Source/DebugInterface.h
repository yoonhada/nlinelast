#ifndef _CDEBUGINTERFACE_H_
#define _CDEBUGINTERFACE_H_

class CDebugInterface : public CSingleton<CDebugInterface>
{
	friend class CSingleton<CDebugInterface>;
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

	D3DXVECTOR3 m_vDebug;
	FLOAT		m_fDebug;

	std::vector<D3DXVECTOR3*> m_vDebugVector;
	std::vector<FLOAT>		 m_fDebugVector;

	INT m_iVectorCount;
	INT	m_iFloatCount;
	
	INT m_iFloatAddCount;
	INT m_iVectorAddCount;

};

#endif