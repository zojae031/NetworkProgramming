#include "stdafx.h"
#include "Client.h"
#include <thread>
#pragma warning(disable:4996)

Client::Client()
{

}


Client::Client(CEdit &box) 
{
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		exit(1);

	// socket()
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	this->box = &box;
}

Client::~Client()
{
}

void Client::setIP(BYTE a, BYTE b, BYTE c, BYTE d)
{
	char chIp[30];
	::wsprintf(chIp, "%d.%d.%d.%d", a, b, c, d);
	serveraddr.sin_addr.s_addr = inet_addr(chIp);
}

void Client::setPort(int port)
{
	serveraddr.sin_port = htons(port);
}

void Client::setId(CString id)
{
	this->id = id;
}

void Client::setNum(CString num)
{
	this->num = num;
}

void Client::connectServer()
{
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) {
		err_quit("connect()");
	}
	
	HANDLE hThread;
	
	hThread = CreateThread(NULL, 0,Client::Receiver,
		(LPVOID)this, 0, NULL);
	if (hThread == NULL) {
		closesocket(sock);
	}
	else { 
		CloseHandle(hThread); 
	}
	
}

void Client::sendData(CString data)
{
	retval = send(sock, data,data.GetLength(), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
}

void Client::err_quit(char * msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	LocalFree(lpMsgBuf);
	exit(1);
}

void Client::err_display(char * msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

DWORD WINAPI Client::Receiver(LPVOID arg)
{
	while (true) {
		Client *c = (Client *)arg;
		c->retval = recv(c->sock, c->buf, BUFSIZE, 0);
		if (c->retval == SOCKET_ERROR) {
			c->err_display("recv()");
		}
		// 받은 데이터 출력
		c->buf[c->retval] = '\0';
		c->message.AppendFormat(c->buf);
		c->box->SetWindowTextA(c->message);
	}
	return 1;
}



