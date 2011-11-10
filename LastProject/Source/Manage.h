#ifndef _CManage_H_
#define _CManage_H_

class CManage
{
private:
	static CManage* _instance;

protected:
	CManage() { _instance = NULL; Clear(); }
	virtual ~CManage() { Release(); }

public:
	CManage* GetInstance()
	{
		if ( NULL == _instance )
		{
			_instance = new CManage;
		}

		return _instance;
	}

	static void DestoryInstance()
	{
		if ( NULL != _instance )
		{
			delete _instance;
			_instance = NULL;
		}
	}

	virtual VOID Clear();
	virtual HRESULT Create( LPDIRECT3DDEVICE9 a_pD3dDevice );
	virtual HRESULT Release();
	virtual VOID Update();
	virtual VOID Render();

private:
};

#endif
