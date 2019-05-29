
// TCP ClientDlg.h: 헤더 파일
//

#pragma once
#include "Client.h"
#define IP 0
#define PORT 1
#define ID 2
#define NUM 3
// CTCPClientDlg 대화 상자
class CTCPClientDlg : public CDialogEx
{
private:
	Client *client;
	CString buf;
	CIPAddressCtrl ip;
	CEdit id, port, num, text;
	CEdit chatBox;
	bool flags[4] = { false };
// 생성입니다.
public:
	CTCPClientDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TCPCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton5();
};
