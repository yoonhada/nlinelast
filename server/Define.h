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
	BLACK = 0,      //  0 : 까망  
	DARK_BLUE,    	//  1 : 어두운 파랑  
	DARK_GREEN,    	//  2 : 어두운 초록  
	DARK_SKY_BLUE,  //  3 : 어두운 하늘  
	DARK_RED,    	//  4 : 어두운 빨강  
	DARK_VOILET,  	//  5 : 어두운 보라  
	DARK_YELLOW,  	//  6 : 어두운 노랑 
	GRAY,      		//  7 : 회색  
	DARK_GRAY,    	//  8 : 어두운 회색 
	BLUE,      		//  9 : 파랑 
	GREEN,      	// 10 : 초록  
	SKY_BLUE,    	// 11 : 하늘  
	RED,      		// 12 : 빨강 
	VOILET,      	// 13 : 보라 
	YELLOW,      	// 14 : 노랑 	
	WHITE,      	// 15 : 하양 
};


#endif