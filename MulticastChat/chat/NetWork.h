// 2019�� 1�б� ��Ʈ��ũ���α׷��� ���� 2��

// ����: ���翵 �й�: 14011038

// �÷���: VS2017
#pragma once
#define BUFSIZE     512
#include "MainDialog.h"
class NetWork
{
private:
	static NetWork *instance;

	int port;
	CString ip;
	CString id;

	SOCKET sock;
	SOCKADDR_IN remoteaddr;

	MainDialog *dlg = NULL;

	CString message;
	bool first = true;
	bool duplicate = false;
	long userPort = 0;


public:
	bool flag[3] = { false,false,false };
private:
	NetWork();
public:
	static NetWork& getInstance();
	virtual ~NetWork();
	void setPort(int port) { instance->port = port; };
	void setIp(CString ip) { instance->ip = ip; };
	void setId(CString id) { instance->id = id; };
	CString getId() { return instance -> id; };
	void setFirst(bool flag) { this->first = flag; };
	void setUserPort(long port) { this->userPort = port; };
	void setDup(bool flag) { this->duplicate = flag; };
	bool getDup() { return this->duplicate; };
	bool getFirst() { return this->first; };
	long getUserPort() { return this->userPort; };
	
	CString& getMsg() { return message; };
	MainDialog& getMainDlg() {
		if (dlg == NULL) {
			dlg = new MainDialog();
		}
		return *dlg;
	};




	void Connect();
	
	void sendMsg(CString data);
	void closeSocket();
	static friend DWORD WINAPI Receiver(LPVOID arg);
	friend void err_quit(char *msg);
	friend void err_display(char *msg);
};

