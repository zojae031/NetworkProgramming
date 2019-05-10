// 2019년 1학기 네트워크프로그래밍 숙제 2번

// 성명: 조재영 학번: 14011038

// 플랫폼: VS2017
#pragma once


// MainDialog 대화 상자

class MainDialog : public CDialogEx
{
	DECLARE_DYNAMIC(MainDialog)
private:
	CString buf;
	CStatic text;
public:
	MainDialog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~MainDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAIN_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedButton2();
};
