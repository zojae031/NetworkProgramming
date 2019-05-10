// 2019년 1학기 네트워크프로그래밍 숙제 2번

// 성명: 조재영 학번: 14011038

// 플랫폼: VS2017
// MainDialog.cpp: 구현 파일
//

#include "stdafx.h"
#include "chat.h"
#include "MainDialog.h"
#include "afxdialogex.h"
#include "NetWork.h"

// MainDialog 대화 상자

IMPLEMENT_DYNAMIC(MainDialog, CDialogEx)

MainDialog::MainDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAIN_DIALOG, pParent)
{
	
}

MainDialog::~MainDialog()
{
	NetWork::getInstance().closeSocket();
}

void MainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXT, text);
}

BOOL MainDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	NetWork::getInstance().sendMsg("님이 채팅방에 입장하셨습니다.");
	return TRUE;
}


BEGIN_MESSAGE_MAP(MainDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &MainDialog::OnBnClickedButton1)
	ON_WM_KEYDOWN()
//	ON_WM_CREATE()
ON_BN_CLICKED(IDC_BUTTON2, &MainDialog::OnBnClickedButton2)
END_MESSAGE_MAP()


// MainDialog 메시지 처리기


void MainDialog::OnBnClickedButton1()
{
	GetDlgItemText(IDC_EDIT1, buf);
	if (NetWork::getInstance().getDup()) {
		NetWork::getInstance().getMsg().Append("대화명 바꾸시죠\r\n");
	}
	NetWork::getInstance().sendMsg(buf);
}


void MainDialog::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case WM_KEYDOWN:
		GetDlgItemText(IDC_EDIT1, buf);
		NetWork::getInstance().sendMsg(buf);
	default:
		break;
	}

	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}



void MainDialog::OnBnClickedButton2()
{
	GetDlgItemText(IDC_EDIT2, buf);
	
	CTime cTime = CTime::GetCurrentTime();
	CString convert;
	convert.AppendFormat(" %s -> %s  (%02d:%02d:%02d)\r\n", NetWork::getInstance().getId(), buf, cTime.GetHour(), cTime.GetMinute(), cTime.GetSecond());
	NetWork::getInstance().sendMsg(convert);
	NetWork::getInstance().setId(buf);
}
