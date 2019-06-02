﻿
// TCP_ServerDlg.h: 헤더 파일
//

#pragma once

#include "Server.h"
// CTCPServerDlg 대화 상자
class CTCPServerDlg : public CDialogEx
{
private : 
	Server *server1,*server2;
	CEdit panel;
	CButton b1, b2, all;
	CButton ok;
	CStatic status;
// 생성입니다.
public:
	CTCPServerDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	~CTCPServerDlg() { delete server1, server2; };
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TCP_SERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedOk();
};
