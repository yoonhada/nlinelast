#pragma once
#ifndef _GAMEEVENTTUTORIAL_H_
#define _GAMEEVENTTUTORIAL_H_

class GameEventTutorial
{
public:
	// ¾À »óÅÂ°ª
	enum TUTORIAL_STATE
	{
		TUTORIAL_COMBO	= 0,
		TUTORIAL_FIRSTAID,
		TUTORIAL_SIGNALLIGHT,
		TUTORIAL_RUNNING,
		TUTORIAL_END
	};

public:
	GameEventTutorial() {}
	virtual ~GameEventTutorial() {}

	virtual VOID	Create() = 0;
	virtual VOID	Update() = 0;
	virtual VOID	Render() = 0;

	virtual DWORD	GetState() = 0;
	virtual VOID	SetState( DWORD _dState ) = 0;

};

#endif