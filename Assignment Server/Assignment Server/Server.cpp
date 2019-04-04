// 2019년 1학기 네트워크프로그래밍 숙제 1번 서버

// 성명: 조재영 학번: 14011038

// 플랫폼: VS2017

// 작동하는 도메인 네임: "www.acmicpc.net","www.naver.com","www.sejong.ac.kr"

#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT 9000
#define BUFSIZE    512

// 소켓 함수 오류 출력 후 종료
void err_quit(char *msg)
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

// 소켓 함수 오류 출력
void err_display(char *msg)
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

// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}
//초기화 함수
void init(int *retval, SOCKET* sock) {
	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	*retval = bind(*sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (*retval == SOCKET_ERROR) err_quit(const_cast<char *>("bind()"));
}
//client 접속 확인 함수
void clientAccept(int *addrlen, SOCKADDR_IN* clientaddr, SOCKET* client_sock, SOCKET* listen_sock) {
	*addrlen = sizeof(*clientaddr);
	*client_sock = accept(*listen_sock, (SOCKADDR *)&clientaddr->sin_addr, addrlen);
	if (*client_sock == INVALID_SOCKET) {
		err_display(const_cast<char *>("accept()"));
	}
}
// 도메인 이름 -> IPv4 주소
BOOL GetIPAddr(char *name, IN_ADDR *addr)
{
	HOSTENT *ptr = gethostbyname(name);
	if (ptr == NULL) {
		err_display(const_cast<char*>("gethostbyname()"));
		return FALSE;
	}
	if (ptr->h_addrtype != AF_INET)
		return FALSE;
	memcpy(addr, ptr->h_addr, ptr->h_length);
	return TRUE;
}
int main(int argc, char *argv[])
{
	const char* host_name[] = {"www.acmicpc.net","www.naver.com","www.sejong.ac.kr"};
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit(const_cast<char *>("socket()"));

	// bind()
	init(&retval, &listen_sock);

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit(const_cast<char *>("listen()"));

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];
	int len;

	while (1) {
		// accept()
		clientAccept(&addrlen, &clientaddr,&client_sock,&listen_sock);


		// 접속한 클라이언트 정보 출력
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// 클라이언트와 데이터 통신
		while (1) {
			// 데이터 받기(고정 길이)
			retval = recvn(client_sock, (char *)&len, sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display(const_cast<char *>("recv()"));
				break;
			} 
			else if (retval == 0)
				break;

			// 데이터 받기(가변 길이)
			retval = recvn(client_sock, buf, len, 0);
			if (retval == SOCKET_ERROR) {
				err_display(const_cast<char *>("recv()"));
				break;
			}
			else if (retval == 0)
				break;
			buf[retval] = '\0';

			//종료 식별코드
			if (!strcmp(buf, "quit"))err_quit(const_cast<char*>("close()"));

			//저장된 주소인지 아닌지 판별
			IN_ADDR addr;
			char * result = NULL;

			for (int i = 0; i < sizeof(host_name) / sizeof(host_name[0]); i++) {
				
				if (strcmp(buf, host_name[i]) == 0) {
					result = buf;
					if (GetIPAddr(const_cast<char*>(result), &addr)) {
						printf("IP 주소 변환 = %s\n", inet_ntoa(addr));
					}
				}
			}


			const char * sendData = "잘못된 정보가 입력 되었습니다.\n";
			if (result == NULL) {
				printf("%s",sendData);
			}
			else {
				// 받은 데이터 출력
				printf("[받은 데이터 : TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
					ntohs(clientaddr.sin_port), buf);
				sendData = inet_ntoa(addr);
			}
			len = strlen(sendData);
		

			//공식명칭, 별명, 모든 IPv4주소 보내기
			
			
			// 데이터 보내기(고정 길이)
			retval = send(client_sock, (char *)&len, sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display(const_cast<char *>("send()"));
				break;
			}

			// 데이터 보내기(가변 길이)
			retval = send(client_sock, sendData, len, 0);
			if (retval == SOCKET_ERROR) {
				err_display(const_cast<char *>("send()"));
				break;
			}

		}

		// closesocket()
		closesocket(client_sock);
		printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		
		break;
	}

	// closesocket()
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}