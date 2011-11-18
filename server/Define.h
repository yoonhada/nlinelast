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
	MSG_CS_LOGON,
	MSG_CS_CHAT,
	MSG_CS_MOVE,
	MSG_CS_UTOM_ATTACK,
	MSG_CS_MTOU_ATTACK,
	MSG_CS_UTOM_ATTACK_ANIMATION,
	MSG_CS_MTOU_ATTACK_ANIMATION,

	MSG_SC_LOGON = 100,
	MSG_SC_INITDATA,
	MSG_SC_NEWUSER,
	MSG_SC_CHAT,
	MSG_SC_MOVE,
	MSG_SC_UTOM_ATTACK,
	MSG_SC_MTOU_ATTACK,
	MSG_SC_UTOM_ATTACK_ANIMATION,
	MSG_SC_MTOU_ATTACK_ANIMATION,
	MSG_SC_DISCONNECT,
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