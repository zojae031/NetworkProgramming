// 2019년 1학기 네트워크프로그래밍 숙제 2번

// 성명: 조재영 학번: 14011038

// 플랫폼: VS2017
// chatDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "chat.h"
#include "chatDlg.h"
#include "afxdialogex.h"
#include "MainDialog.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#define MULTICAST 0
#define PORT 1
#define ID 2
#define DENY 3
#endif

const char* CchatDlg::error[] = { "잘못된 멀티캐스트 주소입니다.", "port 번호가 올바르지 않습니다.", "ID를 입력하세요.","입력정보를 확인하세요." };

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CchatDlg 대화 상자



CchatDlg::CchatDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHAT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CchatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, edit1);
	DDX_Control(pDX, IDC_EDIT2, edit2);
	DDX_Control(pDX, IDC_EDIT3, edit3);
	DDX_Control(pDX, IDC_TEXT, text);
}

BEGIN_MESSAGE_MAP(CchatDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CchatDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CchatDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CchatDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDOK, &CchatDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CchatDlg 메시지 처리기

BOOL CchatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CchatDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CchatDlg::OnPaint()
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
HCURSOR CchatDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CchatDlg::OnBnClickedButton1()
{
	int cnt = 0;
	int pos = 0;
	GetDlgItemText(IDC_EDIT1, buf);
	if (buf == "") {
		text.SetWindowTextA(error[MULTICAST]);
		return;
	}
	CString token;
	bool firstFlag = true;
	
	while ((token = buf.Tokenize(".", pos)) !="") {
		cnt++;
		int intTok = atoi(token);

		if (firstFlag) {//첫번째 검사
			if (!(intTok >= 224 && intTok <= 239)) {
				text.SetWindowTextA(error[MULTICAST]);
				NetWork::getInstance().flag[0] = false;
				return;
			}
			firstFlag = false;
		}

		if (!(intTok >= 0 && intTok <= 255)) {
			text.SetWindowTextA(error[MULTICAST]);
			NetWork::getInstance().flag[0] = false;
			return;
		}

	}
	if (cnt == 4) {
		text.SetWindowTextA("mul OK");
		NetWork::getInstance().flag[0] = true;
		NetWork::getInstance().setIp(buf);
	}
	else {
		NetWork::getInstance().flag[0] = false;
		text.SetWindowTextA(error[MULTICAST]);
	}	
}


void CchatDlg::OnBnClickedButton3()
{
	//0 ~ 65535
	
	GetDlgItemText(IDC_EDIT2, buf);
	
	int portNum = atoi(buf);
	if (portNum > 0 && portNum <= 65535) {
		text.SetWindowTextA("port OK");
		NetWork::getInstance().setPort(portNum);
		NetWork::getInstance().flag[1] = true;
	}
	else {
		text.SetWindowTextA(error[PORT]);
		NetWork::getInstance().flag[1] = false;
		return;
	}

	
}


void CchatDlg::OnBnClickedButton2()
{
	GetDlgItemText(IDC_EDIT3, buf);
	if(buf!=""){ 
		NetWork::getInstance().setId(buf);
		text.SetWindowTextA("ID OK");
		NetWork::getInstance().flag[2] = true;
	}
	else {
		text.SetWindowTextA(error[ID]);
		NetWork::getInstance().flag[2] = false;
	}
	
}


void CchatDlg::OnBnClickedOk()
{
	int cnt = 0;
	for (int i = 0; i < 3; i++) {
		if (NetWork::getInstance().flag[i]) {
			cnt++;
		}
	}
	if (cnt == 3) {
		NetWork::getInstance().Connect();
		NetWork::getInstance().getMainDlg().DoModal();
		CDialogEx::OnOK();
	}
	else {
		text.SetWindowTextA(error[DENY]);
	}
	
	
	
}
