#include "stdafx.h"

#include "UI_Manager.h"
#include "UI_PacketState.h"
#include "UI_ConnectionList.h"


CUI_Manager::CUI_Manager()
{
	UI_ConnectionList	= NULL;
	UI_PacketState		= NULL;
	CurrentUI			= NULL;
}


CUI_Manager::~CUI_Manager()
{
	SAFE_DELETE( UI_ConnectionList );
	SAFE_DELETE( UI_PacketState );
}


VOID CUI_Manager::Initialize()
{
	UI_ConnectionList = new CUI_ConnectionList;
	UI_PacketState = new CUI_PacketState;
}


VOID CUI_Manager::Draw()
{
	CurrentUI->Draw();
}