// 2019년 1학기 네트워크프로그래밍 숙제 2번

// 성명: 조재영 학번: 14011038

// 플랫폼: VS2017

// chatDlg.h: 헤더 파일
//
#include "NetWork.h"
#pragma once


// CchatDlg 대화 상자
class CchatDlg : public CDialogEx
{
// 생성입니다.
public:
	CchatDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	const static  char* error[4];
	
private :
	CString buf;
	CEdit edit1, edit2, edit3;
	CStatic text;
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHAT_DIALOG };
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
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedOk();
};
