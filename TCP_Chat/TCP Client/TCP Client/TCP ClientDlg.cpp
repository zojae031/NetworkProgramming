// 2019년 1학기 네트워크프로그래밍 숙제 3번

// 성명: 조재영 학번: 14011038

// 플랫폼: VS2017
// TCP ClientDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "TCP Client.h"
#include "TCP ClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTCPClientDlg 대화 상자



CTCPClientDlg::CTCPClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TCPCLIENT_DIALOG, pParent){
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CTCPClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, ip);
	DDX_Control(pDX, IDC_EDIT1, port);
	DDX_Control(pDX, IDC_EDIT2, id);
	DDX_Control(pDX, IDC_EDIT3, num);
	DDX_Control(pDX, IDC_EDIT4, chatBox);
	DDX_Control(pDX, IDC_EDIT5, text);
	ip.SetAddress(0, 0, 0, 0);
}

BEGIN_MESSAGE_MAP(CTCPClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CTCPClientDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTCPClientDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CTCPClientDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON3, &CTCPClientDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDOK, &CTCPClientDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON5, &CTCPClientDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDCANCEL, &CTCPClientDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CTCPClientDlg 메시지 처리기

BOOL CTCPClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	client = new Client(chatBox,text);

	

	

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CTCPClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CTCPClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTCPClientDlg::OnBnClickedButton1()
{
	BYTE a, b, c, d;
	ip.GetAddress(a,b,c,d);
	if (a == NULL && b == NULL && c == NULL && d == NULL) {
		MessageBox(_T("아이피 잘못입력"), MB_OK);
		flags[IP] = false;
	}
	else {
		client->setIP(a, b, c, d);
		MessageBox(_T("성공"), MB_OK);
		flags[IP] = true;
	}
	
}


void CTCPClientDlg::OnBnClickedButton2()
{
	CString tmp;
	int int_port=0;
	port.GetWindowText(tmp);
	int_port = atoi(tmp);
	if (int_port == 0 && strcmp(tmp, "0") != 0) {
		int_port = -1;
	}
	if (int_port<= 65535 && int_port >=0) {
		
		client->setPort(int_port);
		MessageBox(_T("성공"), MB_OK);
		flags[PORT] = true;
	}
	else {
		MessageBox(_T("포트 잘못입력"), MB_OK);
		flags[PORT] = false;
	}
	
}


void CTCPClientDlg::OnBnClickedButton4()
{
	CString user;
	id.GetWindowText(user);
	int cnt = 0;
	for (int i = 0; i < 4; i++) {//플래그 돌리기
		if (flags[i])cnt++;
	}

	if (!text.IsWindowEnabled() && cnt == 4) {//text 비활성화고 모든 플래그 on이면
		if (client->getId() != user) {
			CString msg = "{" + client->getId() + "} -> [" + user+"]\r\n";
			text.EnableWindow(true);
			client->setId(user);
			client->sendData(msg);
		}
	}
	else {
		if (user == "") {
			MessageBox(_T("아이디 잘못 입력"), MB_OK);
			flags[ID] = false;
		}
		else {
			client->setId(user);
			flags[ID] = true;
			MessageBox(_T("성공"), MB_OK);
		}
	}
}


void CTCPClientDlg::OnBnClickedButton3()
{
	CString number;
	num.GetWindowTextA(number);
	if (number == "") {
		MessageBox(_T("방 번호 잘못 입력"), MB_OK);
		flags[NUM] = false;
	}
	else {
		int compare = atoi(number);
		if (compare == 1 || compare == 2) {
			flags[NUM] = true;
			MessageBox(_T("성공"), MB_OK);
			client->setPort(client->getPort()+compare-1);
			client->setNum(number);
		}
		else {
			MessageBox(_T("1 또는 2를 입력하시오."), MB_OK);
		}
		
		
	}
}


void CTCPClientDlg::OnBnClickedOk()
{
	//TODO FLAG CHECK
	for (int i = 0; i < 4; i++) {
		if (!flags[i]) {
			MessageBox(_T("입력 정보를 확인하시오."), MB_OK);
			return;
		}
	}
	client->connectServer();
	text.EnableWindow(true);
	GetDlgItem(IDOK)->EnableWindow(false);

}


void CTCPClientDlg::OnBnClickedButton5()
{
	CString t;
	text.GetWindowText(t);
	if(t!=""){ 
		CString message;
		message.Format("%s : %s \r\n", client->getId(), t);
		client->sendData(message); 
	}
	text.SetWindowText("");
	
}


void CTCPClientDlg::OnBnClickedCancel()
{
	CString logout;
	logout.Format("%s님이 채팅방에서 나가셨습니다.\r\n", client->getId());
	client->sendData(logout);
	CDialogEx::OnCancel();
}
