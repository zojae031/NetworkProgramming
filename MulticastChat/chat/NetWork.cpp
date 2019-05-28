// 2019�� 1�б� ��Ʈ��ũ���α׷��� ���� 2��

// ����: ���翵 �й�: 14011038

// �÷���: VS2017
#include "stdafx.h"
#include "NetWork.h"
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include"resource.h"
#pragma warning(disable:4996)

#include <thread>
NetWork *NetWork::instance = NULL;
NetWork::NetWork()
{

}


NetWork& NetWork::getInstance()
{
	if (instance == NULL) {
		instance = new NetWork();
	}
	return *instance;
}

NetWork::~NetWork()
{
	delete dlg;
	delete instance;
}

void NetWork::Connect()
{
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;

	// socket()
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// ��Ƽĳ��Ʈ TTL ����
	int ttl = 2;
	int retval = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL,
		(char *)&ttl, sizeof(ttl));
	if (retval == SOCKET_ERROR) err_quit("setsockopt()");

	// ���� �ּ� ����ü �ʱ�ȭ
	ZeroMemory(&remoteaddr, sizeof(remoteaddr));
	remoteaddr.sin_family = AF_INET;
	remoteaddr.sin_addr.s_addr = inet_addr(ip);
	remoteaddr.sin_port = htons(port);


	//���ù� ������ ����
	HANDLE hThread;
	hThread = CreateThread(NULL, 0, Receiver,
		(LPVOID)sock, 0, NULL);
	if (hThread == NULL) { closesocket(sock); }
	else { CloseHandle(hThread); }



}

void NetWork::sendMsg(CString data)
{
	// ��Ƽĳ��Ʈ ������ ������

		// ������ �Է�
	if (data.GetLength() == 0) {
		return;
	}
	int retval = 0;
	// '\n' ���� ����
	int len = strlen(data);
	if (data[len - 1] == '\n')data.SetAt(len - 1, '\0');



	
	// ������ ������
	retval = sendto(sock, id, strlen(id), 0,
		(SOCKADDR *)&remoteaddr, sizeof(remoteaddr));
	if (retval == SOCKET_ERROR) {
		err_display("sendto()");
	}

	retval = sendto(sock, data, strlen(data), 0,
		(SOCKADDR *)&remoteaddr, sizeof(remoteaddr));
	if (retval == SOCKET_ERROR) {
		err_display("sendto()");
	}
	NetWork::getInstance().getMainDlg().GetDlgItem(IDC_EDIT1)->SetWindowTextA("");




}
void NetWork::closeSocket()
{
	// closesocket()
	closesocket(sock);

	// ���� ����
	WSACleanup();
}


void err_quit(char * msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void err_display(char * msg)
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

DWORD __stdcall Receiver(LPVOID arg)
{

	BOOL optval = TRUE;
	int retval = 0;
	SOCKADDR_IN localaddr;

	
	char buf[BUFSIZE + 1];
	memset(buf, 0, sizeof(buf));
	char name[50];
	struct ip_mreq mreq;
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) {
		err_quit("socket()");
	}

	// SO_REUSEADDR �ɼ� ����
	retval = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(optval));
	if (retval == SOCKET_ERROR) {
		err_quit("setsockopt()");
	}

	// bind()
	ZeroMemory(&localaddr, sizeof(localaddr));
	localaddr.sin_family = AF_INET;
	localaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	localaddr.sin_port = htons(NetWork::getInstance().port);

	retval = bind(sock, (SOCKADDR *)&localaddr, sizeof(localaddr));


	if (retval == SOCKET_ERROR) err_quit("bind()");

	// ��Ƽĳ��Ʈ �׷� ����
	mreq.imr_multiaddr.s_addr = inet_addr(NetWork::getInstance().ip);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	retval = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
		(char *)&mreq, sizeof(mreq));


	// ������ ��ſ� ����� ����

	SOCKADDR_IN peeraddr;

	// ��Ƽĳ��Ʈ ������ �ޱ�
	while (1) {
		// ������ �ޱ�
		int addrlen = sizeof(peeraddr);

		retval = recvfrom(sock, name, 50, 0,
			(SOCKADDR *)&peeraddr, &addrlen);
		if (retval == SOCKET_ERROR) {
			continue;
		}
		name[retval] = '\0';

		retval = recvfrom(sock, buf, BUFSIZE, 0,
			(SOCKADDR *)&peeraddr, &addrlen);
		if (retval == SOCKET_ERROR) {
			continue;
		}
		// ���� ������ ���
		buf[retval] = '\0';

		if (NetWork::getInstance().getFirst()) {
			NetWork::getInstance().setUserPort(ntohs(peeraddr.sin_port));
			NetWork::getInstance().setFirst(false);
		}
	
		if (strcmp(NetWork::getInstance().getId(), name) == 0 && NetWork::getInstance().getUserPort() != ntohs(peeraddr.sin_port)) {
			NetWork::getInstance().setDup(true);
		}
		else {
			NetWork::getInstance().setDup(false);
		}
		
		CTime cTime = CTime::GetCurrentTime(); // ���� �ý������κ��� ��¥ �� �ð��� ��� �´�.
		NetWork::getInstance().getMsg().AppendFormat("[%s:%d ] %s : %s  (%02d:%02d:%02d)\r\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port), name, buf, cTime.GetHour(), cTime.GetMinute(), cTime.GetSecond());

		NetWork::getInstance().getMainDlg().GetDlgItem(IDC_TEXT)->SetWindowTextA(NetWork::getInstance().getMsg());

		
		
	}

	// ��Ƽĳ��Ʈ �׷� Ż��
	retval = setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP,
		(char *)&mreq, sizeof(mreq));
	if (retval == SOCKET_ERROR)

		// closesocket()
		closesocket(sock);

	// ���� ����
	WSACleanup();
	return 0;
}
