#pragma once
#define BUFSIZE 512
class SocketInfo
{
public :
	SOCKET sock;
	char buf[BUFSIZE+1];
	int recvbytes;
	int sendbytes;
	CString id;
public:
	SocketInfo();
	~SocketInfo();
};

