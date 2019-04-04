// 2019�� 1�б� ��Ʈ��ũ���α׷��� ���� 1�� ����

// ����: ���翵 �й�: 14011038

// �÷���: VS2017

// �۵��ϴ� ������ ����: "www.acmicpc.net","www.naver.com","www.sejong.ac.kr"

#define _WINSOCK_DEPRECATED_NO_WARNINGS // �ֽ� VC++ ������ �� ��� ����
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT 9000
#define BUFSIZE    512

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
//�ʱ�ȭ �Լ�
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
//client ���� Ȯ�� �Լ�
void clientAccept(int *addrlen, SOCKADDR_IN* clientaddr, SOCKET* client_sock, SOCKET* listen_sock) {
	*addrlen = sizeof(*clientaddr);
	*client_sock = accept(*listen_sock, (SOCKADDR *)&clientaddr->sin_addr, addrlen);
	if (*client_sock == INVALID_SOCKET) {
		err_display(const_cast<char *>("accept()"));
	}
}
// ������ �̸� -> IPv4 �ּ�
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

	// ���� �ʱ�ȭ
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

	// ������ ��ſ� ����� ����
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];
	int len;

	while (1) {
		// accept()
		clientAccept(&addrlen, &clientaddr,&client_sock,&listen_sock);


		// ������ Ŭ���̾�Ʈ ���� ���
		printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// Ŭ���̾�Ʈ�� ������ ���
		while (1) {
			// ������ �ޱ�(���� ����)
			retval = recvn(client_sock, (char *)&len, sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display(const_cast<char *>("recv()"));
				break;
			} 
			else if (retval == 0)
				break;

			// ������ �ޱ�(���� ����)
			retval = recvn(client_sock, buf, len, 0);
			if (retval == SOCKET_ERROR) {
				err_display(const_cast<char *>("recv()"));
				break;
			}
			else if (retval == 0)
				break;
			buf[retval] = '\0';

			//���� �ĺ��ڵ�
			if (!strcmp(buf, "quit"))err_quit(const_cast<char*>("close()"));

			//����� �ּ����� �ƴ��� �Ǻ�
			IN_ADDR addr;
			char * result = NULL;

			for (int i = 0; i < sizeof(host_name) / sizeof(host_name[0]); i++) {
				
				if (strcmp(buf, host_name[i]) == 0) {
					result = buf;
					if (GetIPAddr(const_cast<char*>(result), &addr)) {
						printf("IP �ּ� ��ȯ = %s\n", inet_ntoa(addr));
					}
				}
			}


			const char * sendData = "�߸��� ������ �Է� �Ǿ����ϴ�.\n";
			if (result == NULL) {
				printf("%s",sendData);
			}
			else {
				// ���� ������ ���
				printf("[���� ������ : TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
					ntohs(clientaddr.sin_port), buf);
				sendData = inet_ntoa(addr);
			}
			len = strlen(sendData);
		

			//���ĸ�Ī, ����, ��� IPv4�ּ� ������
			
			
			// ������ ������(���� ����)
			retval = send(client_sock, (char *)&len, sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display(const_cast<char *>("send()"));
				break;
			}

			// ������ ������(���� ����)
			retval = send(client_sock, sendData, len, 0);
			if (retval == SOCKET_ERROR) {
				err_display(const_cast<char *>("send()"));
				break;
			}

		}

		// closesocket()
		closesocket(client_sock);
		printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		
		break;
	}

	// closesocket()
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();
	return 0;
}