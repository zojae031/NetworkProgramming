// 2019�� 1�б� ��Ʈ��ũ���α׷��� ���� 1�� Ŭ���̾�Ʈ

// ����: ���翵 �й�: 14011038

// �÷���: VS2017

// �۵��ϴ� ������ ����: "www.google.com","www.naver.com","www.sejong.ac.kr"

#define _WINSOCK_DEPRECATED_NO_WARNINGS // �ֽ� VC++ ������ �� ��� ����
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    50

// ���� �Լ� ���� ��� �� ����
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

// ���� �Լ� ���� ���
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
// ����� ���� ������ ���� �Լ�
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

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit(const_cast<char *>("socket()"));

	init(&retval, &sock);

	// ������ ��ſ� ����� ����
	char buf[BUFSIZE];
	int len;

	// ������ ������ ���
	while(1){

		// ������ �Է�
		printf("\n[���� ������] ");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
			break;
		len = strlen(buf);
		buf[(len--) - 1] = '\0';



		// ������ ������(���� ����)
		retval = send(sock, (char *)&len, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display(const_cast<char *>("send()"));
			break;
		}

		// ������ ������(���� ����)
		retval = send(sock, buf, len, 0);
		if (retval == SOCKET_ERROR) {
			err_display(const_cast<char *>("send()"));
			break;
		}

		//���� üũ
		if (!strcmp(buf, "quit")) {
			printf("�����մϴ�\n");
			break;
		}
		memset(buf, 0, sizeof(buf));
		// ������ �ޱ� (��������)
		retval = recvn(sock, (char *)&len, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display(const_cast<char *>("recv()"));
			break;
		}
		else if (retval == 0)
			break;

	

		// ������ �ޱ�(���� ����)
		retval = recvn(sock, buf, len, 0);
		if (retval == SOCKET_ERROR) {
			err_display(const_cast<char *>("recv()"));
			break;
		}
		else if (retval == 0)
			break;

		buf[retval] = '\0';


		// ���� ������ ���
		if (strcmp(buf, "�߸��� ������ �Է� �Ǿ����ϴ�.\n") == 0) {
			printf("[���� ������] %s\n", buf);
		}
		else {
			hostent *host = gethostbyname(buf);
			printf("Host_Name : %s\n", host->h_name);
			for (int i = 0; host->h_addr_list[i]; i++) {
				printf("IP�ּ� : ");
				puts(inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
			}

			for (int i = 0; host->h_aliases[i]; i++) {
				printf("���� : ");
				puts(host->h_aliases[i]);
			}
		}
		
		


	}

	// closesocket()
	closesocket(sock);

	// ���� ����
	WSACleanup();
	return 0;
}