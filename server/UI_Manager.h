#ifndef _UI_MANAGER_H_
#define _UI_MANAGER_H_

#include "UI.h"


class CUI_Manager
{
private:
	CUI* UI_ConnectionList;
	CUI* UI_PacketState;
	CUI* CurrentUI;


public:
	CUI_Manager();
	~CUI_Manager();

	VOID Initialize();

	VOID Draw();
};


#endif