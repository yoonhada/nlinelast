#ifndef _DEFINE_H_
#define _DEFINE_H_


enum OVERLAPPED_MODE
{
	RECV_POSTED = 0,
	SEND_POSTED,
};


struct OVERLAPPED_PLUS : public OVERLAPPED
{
	VOID*	m_pClient;
	INT		m_Mode;		// 0: Recv	1: Send
};


#define HEADER_SIZE 4
#define BUFFER_SIZE 4096 * 2
#define PACKET_SIZE 2048


// Message ID
enum MESSAGE_ID
{
	MSG_START = 0,

	MSG_LOGON,
	MSG_INIT,
	MSG_CHANGE_HOST,
	MSG_ADD_USER,
	MSG_READY,
	MSG_START_BUTTON_STATE,
	MSG_GAME_START,
	MSG_LODING_COMPLETE,
	MSG_CHANGE_SCENE,
	MSG_CLIENT_DISCONNECT,
	MSG_SERVER_CLOSE,

	MSG_EVENT_STATE,
	MSG_EVENT_COMBO_INFO,
	MSG_EVENT_COMBO_SLOT_STATE,
	MSG_EVENT_COMBO_RESULT,
	MSG_EVENT_HEAL,

	MSG_CHAT,
	MSG_PLAYER_MOVE,
	MSG_MONSTER_MOVE,

	MSG_UTOM_ATTACK,
	MSG_MTOU_ATTACK,
	MSG_EVENT_ATTACK,

	MSG_PLAYER_ATTACK_ANIMATION,
	MSG_MONSTER_ATTACK_ANIMATION,
	MSG_MONSTER_ATTACK_ANIMATION2,
	MSG_MONSTER_LOCKON,

	MSG_GAME_RESULT,

	MSG_END,
};


// Color
enum _Color { 
	BLACK = 0,      //  0 : ���  
	DARK_BLUE,    	//  1 : ��ο� �Ķ�  
	DARK_GREEN,    	//  2 : ��ο� �ʷ�  
	DARK_SKY_BLUE,  //  3 : ��ο� �ϴ�  
	DARK_RED,    	//  4 : ��ο� ����  
	DARK_VOILET,  	//  5 : ��ο� ����  
	DARK_YELLOW,  	//  6 : ��ο� ��� 
	GRAY,      		//  7 : ȸ��  
	DARK_GRAY,    	//  8 : ��ο� ȸ�� 
	BLUE,      		//  9 : �Ķ� 
	GREEN,      	// 10 : �ʷ�  
	SKY_BLUE,    	// 11 : �ϴ�  
	RED,      		// 12 : ���� 
	VOILET,      	// 13 : ���� 
	YELLOW,      	// 14 : ��� 	
	WHITE,      	// 15 : �Ͼ� 
};


#endif