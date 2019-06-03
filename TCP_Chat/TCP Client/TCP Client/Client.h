#pragma once
// 2019�� 1�б� ��Ʈ��ũ���α׷��� ���� 3��

// ����: ���翵 �й�: 14011038

// �÷���: VS2017

#define BUFSIZE    512
class Client 
{
private :
	SOCKADDR_IN serveraddr;
	SOCKET sock;
	CString ip,id, num;
	CEdit *box,*text;
	CString message;
	char buf[BUFSIZE];
	int port;
	int retval = 0;

public:
	Client();
	Client(CEdit &box,CEdit &text);
	~Client();
	void setIP(BYTE a, BYTE b, BYTE c, BYTE d);
	void setPort(int port);
	void setId(CString id);
	void setNum(CString num);
	void connectServer();
	void sendData(CString data);


	CString getId() { return id; };
	int getPort() { return port; };
	void err_quit(char*msg);
	void err_display(char *msg);
	static DWORD WINAPI Receiver(LPVOID arg);
};


