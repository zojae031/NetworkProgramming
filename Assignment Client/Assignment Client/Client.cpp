// 2019년 1학기 네트워크프로그래밍 숙제 1번 클라이언트

// 성명: 조재영 학번: 14011038

// 플랫폼: VS2017

// 작동하는 도메인 네임: "www.google.com","www.naver.com","www.sejong.ac.kr, "www.github.com", "www.daum.net"

#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    50

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
void init(int *retval,SOCKET* sock) {
	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	*retval = connect(*sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (*retval == SOCKET_ERROR) err_quit(const_cast<char *>("connect()"));
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
int main(int argc, char *argv[])
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit(const_cast<char *>("socket()"));

	init(&retval, &sock);

	// 데이터 통신에 사용할 변수
	char buf[BUFSIZE];
	int len;

	// 서버와 데이터 통신
	while(1){

		// 데이터 입력
		printf("\n[보낼 데이터] ");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
			break;
		len = strlen(buf);
		buf[(len--) - 1] = '\0';



		// 데이터 보내기(고정 길이)
		retval = send(sock, (char *)&len, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display(const_cast<char *>("send()"));
			break;
		}

		// 데이터 보내기(가변 길이)
		retval = send(sock, buf, len, 0);
		if (retval == SOCKET_ERROR) {
			err_display(const_cast<char *>("send()"));
			break;
		}

		//종료 체크
		if (!strcmp(buf, "quit")) {
			printf("종료합니다\n");
			break;
		}
		memset(buf, 0, sizeof(buf));

		while (strcmp(buf, "end") != 0) {

			// 데이터 받기 (고정길이)
			retval = recvn(sock, (char *)&len, sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display(const_cast<char *>("recv()"));
				break;
			}
			else if (retval == 0)
				break;



			// 데이터 받기(가변 길이)
			retval = recvn(sock, buf, len, 0);
			if (retval == SOCKET_ERROR) {
				err_display(const_cast<char *>("recv()"));
				break;
			}
			else if (retval == 0)
				break;

			buf[retval] = '\0';


			// 받은 데이터 출력
			if (strcmp(buf, "잘못된 정보가 입력 되었습니다.\n") == 0) {
				printf("[받은 데이터] %s\n", buf);
				break;
			}
			else {
				printf("%s\n", buf);
	
			}
		}
		


	}

	// closesocket()
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}