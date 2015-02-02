
// X_ImageSearchingDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CX_ImageSearchingDlg 对话框
class CX_ImageSearchingDlg : public CDialogEx
{
// 构造
public:
	CX_ImageSearchingDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_X_IMAGESEARCHING_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl x_listControl;
	CStatic x_picture;
//	afx_msg void OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCm();
};

extern void ListFill(CListCtrl& c);//初始化列表控件，将所有图片导入到列表控件中extern void outputTheResult();//输出查找结果
