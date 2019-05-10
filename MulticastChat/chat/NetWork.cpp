// 2019년 1학기 네트워크프로그래밍 숙제 2번

// 성명: 조재영 학번: 14011038

// 플랫폼: VS2017
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
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;

	// socket()
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// 멀티캐스트 TTL 설정
	int ttl = 2;
	int retval = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL,
		(char *)&ttl, sizeof(ttl));
	if (retval == SOCKET_ERROR) err_quit("setsockopt()");

	// 소켓 주소 구조체 초기화
	ZeroMemory(&remoteaddr, sizeof(remoteaddr));
	remoteaddr.sin_family = AF_INET;
	remoteaddr.sin_addr.s_addr = inet_addr(ip);
	remoteaddr.sin_port = htons(port);


	//리시버 스레드 생성
	HANDLE hThread;
	hThread = CreateThread(NULL, 0, Receiver,
		(LPVOID)sock, 0, NULL);
	if (hThread == NULL) { closesocket(sock); }
	else { CloseHandle(hThread); }



}

void NetWork::sendMsg(CString data)
{
	// 멀티캐스트 데이터 보내기

		// 데이터 입력
	if (data.GetLength() == 0) {
		return;
	}
	int retval = 0;
	// '\n' 문자 제거
	int len = strlen(data);
	if (data[len - 1] == '\n')data.SetAt(len - 1, '\0');



	
	// 데이터 보내기
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

	// 윈속 종료
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
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) {
		err_quit("socket()");
	}

	// SO_REUSEADDR 옵션 설정
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

	// 멀티캐스트 그룹 가입
	mreq.imr_multiaddr.s_addr = inet_addr(NetWork::getInstance().ip);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	retval = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
		(char *)&mreq, sizeof(mreq));


	// 데이터 통신에 사용할 변수

	SOCKADDR_IN peeraddr;

	// 멀티캐스트 데이터 받기
	while (1) {
		// 데이터 받기
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
		// 받은 데이터 출력
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
		
		CTime cTime = CTime::GetCurrentTime(); // 현재 시스템으로부터 날짜 및 시간을 얻어 온다.
		NetWork::getInstance().getMsg().AppendFormat("[%s:%d ] %s : %s  (%02d:%02d:%02d)\r\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port), name, buf, cTime.GetHour(), cTime.GetMinute(), cTime.GetSecond());

		NetWork::getInstance().getMainDlg().GetDlgItem(IDC_TEXT)->SetWindowTextA(NetWork::getInstance().getMsg());

		
		
	}

	// 멀티캐스트 그룹 탈퇴
	retval = setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP,
		(char *)&mreq, sizeof(mreq));
	if (retval == SOCKET_ERROR)

		// closesocket()
		closesocket(sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}
