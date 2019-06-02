#pragma once
#include"SocketInfo.h"
#define SERVERPORT 9000
#define BUFSIZE    512

class Server
{
public:
	int nTotalSockets = 0;
	SocketInfo *SocketInfoArray[FD_SETSIZE];
	int retval;
	WSADATA wsa;
	SOCKET listen_sock;
	SOCKADDR_IN serveraddr;
	//데이터 통신에 사용할 변수
	FD_SET rset, wset;
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	HANDLE hThread;
	//
	int idx;
public:
	Server();
	Server(int idx);
	~Server();
	void openServer();
	static DWORD WINAPI serverThread1(LPVOID arg);
	static DWORD WINAPI serverThread2(LPVOID arg);
	BOOL AddSocketInfo(SOCKET sock);
	void RemoveSocketInfo(int nIndex);
};

