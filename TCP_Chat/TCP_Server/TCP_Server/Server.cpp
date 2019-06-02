#include "stdafx.h"
#include "Server.h"
#pragma warning (disable:4996)

Server::Server()
{
	//init winsock
	WSAStartup(MAKEWORD(2, 2), &wsa);
	//socket()
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);

}

Server::Server(int idx) :Server()
{
	this->idx = idx;
	// bind()
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT + idx - 1);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	//listen()
	retval = listen(listen_sock, SOMAXCONN);

	// �ͺ��ŷ �������� ��ȯ
	u_long on = 1;
	retval = ioctlsocket(listen_sock, FIONBIO, &on);
}


Server::~Server()
{
	CloseHandle(hThread);
	WSACleanup();
}

void Server::openServer()
{
	if (idx == 1) {
		hThread = CreateThread(NULL, 0, Server::serverThread1,
			(LPVOID)this, 0, NULL);
		if (hThread == NULL) {
			closesocket(this->listen_sock);
		}
		else {
			CloseHandle(hThread);
		}
	}
	else {
		hThread = CreateThread(NULL, 0, Server::serverThread2,
			(LPVOID)this, 0, NULL);
		if (hThread == NULL) {
			closesocket(this->listen_sock);
		}
		else {
			CloseHandle(hThread);
		}
	}
}

DWORD __stdcall Server::serverThread1(LPVOID arg)
{
	Server *s = (Server *)arg;
	while (1) {
		// ���� �� �ʱ�ȭ
		FD_ZERO(&s->rset);
		FD_ZERO(&s->wset);
		FD_SET(s->listen_sock, &s->rset);
		for (int i = 0; i < s->nTotalSockets; i++) {
			if (s->SocketInfoArray[i]->recvbytes > s->SocketInfoArray[i]->sendbytes)
				FD_SET(s->SocketInfoArray[i]->sock, &s->wset);
			else
				FD_SET(s->SocketInfoArray[i]->sock, &s->rset);
		}

		// select()
		s->retval = select(0, &s->rset, &s->wset, NULL, NULL);
		if (s->retval == SOCKET_ERROR) exit(1);

		// ���� �� �˻�(1): Ŭ���̾�Ʈ ���� ����
		if (FD_ISSET(s->listen_sock, &s->rset)) {
			s->addrlen = sizeof(clientaddr);
			s->client_sock = accept(s->listen_sock, (SOCKADDR *)&s->clientaddr, &s->addrlen);
			if (s->client_sock == INVALID_SOCKET) {
				exit(1);
			}
			else {
				s->AddSocketInfo(s->client_sock);
			}
		}

		// ���� �� �˻�(2): ������ ���
		for (int i = 0; i < s->nTotalSockets; i++) {
			SocketInfo *ptr = s->SocketInfoArray[i];
			if (FD_ISSET(ptr->sock, &s->rset)) {
				// ������ �ޱ�
				s->retval = recv(ptr->sock, ptr->buf, BUFSIZE, 0);
				if (s->retval == SOCKET_ERROR) {
					s->RemoveSocketInfo(i);
					continue;
				}
				else if (s->retval == 0) {
					s->RemoveSocketInfo(i);
					continue;
				}
				ptr->recvbytes = s->retval;
				// ���� ������ ���
				ptr->buf[s->retval] = '\0';
				CString tmp = ptr->buf;
				int startIndex = tmp.Find("{");
				int lastIndex = tmp.Find("}");
				if (lastIndex > 0) {
					int idx = 0;
					for (int l = startIndex+1; l < lastIndex; l++) {
						tmp.SetAt(idx++, ptr->buf[l]);
					}
					tmp.SetAt(idx, '\0');
					ptr->id = tmp;
				}

			
			}
			if (FD_ISSET(ptr->sock, &s->wset)) {
				// ������ ������
				for (int j = 0; j < s->nTotalSockets; j++) {  // ���� �����ڿ��� �߼�
					SocketInfo *sptr = s->SocketInfoArray[j];
					s->retval = send(sptr->sock, ptr->buf + ptr->sendbytes,
						ptr->recvbytes - ptr->sendbytes, 0);

					if (s->retval == SOCKET_ERROR) {
						
						s->RemoveSocketInfo(i);
						continue;
					}
				}
				ptr->sendbytes += s->retval;
				if (ptr->recvbytes == ptr->sendbytes) {
					ptr->recvbytes = ptr->sendbytes = 0;
				}



			}
		}
	}
}

DWORD __stdcall Server::serverThread2(LPVOID arg)
{
	Server *s = (Server *)arg;
	while (1) {
		// ���� �� �ʱ�ȭ
		FD_ZERO(&s->rset);
		FD_ZERO(&s->wset);
		FD_SET(s->listen_sock, &s->rset);
		for (int i = 0; i < s->nTotalSockets; i++) {
			if (s->SocketInfoArray[i]->recvbytes > s->SocketInfoArray[i]->sendbytes)
				FD_SET(s->SocketInfoArray[i]->sock, &s->wset);
			else
				FD_SET(s->SocketInfoArray[i]->sock, &s->rset);
		}

		// select()
		s->retval = select(0, &s->rset, &s->wset, NULL, NULL);
		if (s->retval == SOCKET_ERROR) exit(1);

		// ���� �� �˻�(1): Ŭ���̾�Ʈ ���� ����
		if (FD_ISSET(s->listen_sock, &s->rset)) {
			s->addrlen = sizeof(clientaddr);
			s->client_sock = accept(s->listen_sock, (SOCKADDR *)&s->clientaddr, &s->addrlen);
			if (s->client_sock == INVALID_SOCKET) {
				exit(1);
			}
			else {
				s->AddSocketInfo(s->client_sock);
			}
		}

		// ���� �� �˻�(2): ������ ���
		for (int i = 0; i < s->nTotalSockets; i++) {
			SocketInfo *ptr = s->SocketInfoArray[i];
			if (FD_ISSET(ptr->sock, &s->rset)) {
				// ������ �ޱ�
				s->retval = recv(ptr->sock, ptr->buf, BUFSIZE, 0);
				if (s->retval == SOCKET_ERROR) {
					s->RemoveSocketInfo(i);
					continue;
				}
				else if (s->retval == 0) {
					s->RemoveSocketInfo(i);
					continue;
				}
				ptr->recvbytes = s->retval;
				// ���� ������ ���
				ptr->buf[s->retval] = '\0';
				CString tmp = ptr->buf;
				int startIndex = tmp.Find("{");
				int lastIndex = tmp.Find("}");
				if (lastIndex > 0) {
					int idx = 0;
					for (int l = startIndex + 1; l < lastIndex; l++) {
						tmp.SetAt(idx++, ptr->buf[l]);
					}
					tmp.SetAt(idx, '\0');
					ptr->id = tmp;
				}
			}
			if (FD_ISSET(ptr->sock, &s->wset)) {
				// ������ ������
				for (int j = 0; j < s->nTotalSockets; j++) {  // ���� �����ڿ��� �߼�
					SocketInfo *sptr = s->SocketInfoArray[j];
					s->retval = send(sptr->sock, ptr->buf + ptr->sendbytes,
						ptr->recvbytes - ptr->sendbytes, 0);

					if (s->retval == SOCKET_ERROR) {

						s->RemoveSocketInfo(i);
						continue;
					}
				}
				ptr->sendbytes += s->retval;
				if (ptr->recvbytes == ptr->sendbytes) {
					ptr->recvbytes = ptr->sendbytes = 0;
				}



			}
		}
	}
}


BOOL Server::AddSocketInfo(SOCKET sock)
{
	if (nTotalSockets >= FD_SETSIZE) {
		printf("[����] ���� ������ �߰��� �� �����ϴ�!\n");
		return FALSE;
	}

	SocketInfo *ptr = new SocketInfo;
	if (ptr == NULL) {
		printf("[����] �޸𸮰� �����մϴ�!\n");
		return FALSE;
	}
	ptr->sock = sock;
	ptr->recvbytes = 0;
	ptr->sendbytes = 0;
	SocketInfoArray[nTotalSockets++] = ptr;

	return TRUE;
}

void Server::RemoveSocketInfo(int nIndex)
{
	SocketInfo *ptr = SocketInfoArray[nIndex];
	closesocket(ptr->sock);
	delete ptr;
	if (nIndex != (nTotalSockets - 1))
		SocketInfoArray[nIndex] = SocketInfoArray[nTotalSockets - 1];
	--nTotalSockets;
}
