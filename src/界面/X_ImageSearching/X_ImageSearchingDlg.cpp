
// X_ImageSearchingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "X_ImageSearching.h"
#include "X_ImageSearchingDlg.h"
#include "afxdialogex.h"
#include <fstream>
#include <iostream>
#include "Rtree_image_search.h"
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

char *selected;
int key = 0;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CX_ImageSearchingDlg 对话框



CX_ImageSearchingDlg::CX_ImageSearchingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CX_ImageSearchingDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CX_ImageSearchingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, x_listControl);
	DDX_Control(pDX, IDC_PICTURE, x_picture);
}

BEGIN_MESSAGE_MAP(CX_ImageSearchingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CX_ImageSearchingDlg::OnNMDblclkList)
ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CX_ImageSearchingDlg::OnNMDblclkList)
ON_BN_CLICKED(IDC_CM, &CX_ImageSearchingDlg::OnBnClickedCm)
END_MESSAGE_MAP()


// CX_ImageSearchingDlg 消息处理程序

BOOL CX_ImageSearchingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	x_listControl.ModifyStyle(0L, LVS_REPORT);
	x_listControl.SetExtendedStyle(LVS_EX_FULLROWSELECT);// 选中整行

	x_listControl.InsertColumn(0, _T("图片库"));

	CRect rect;
    x_listControl.GetClientRect(&rect);
    x_listControl.SetColumnWidth(0, rect.Width( ));  

	

	ListFill(x_listControl);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CX_ImageSearchingDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CX_ImageSearchingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
	if(!key)
	{
		CImage image;
		image.Load(_T("start.jpg"));
		CRect rect;
		int cx = image.GetWidth();
		int cy = image.GetHeight();

		GetDlgItem(IDC_PICTURE)->GetWindowRect(&rect);
		ScreenToClient(&rect);
		GetDlgItem(IDC_PICTURE)->MoveWindow(rect.left,rect.top,cx,cy,TRUE);

		CWnd *pWnd = GetDlgItem(IDC_PICTURE);
		pWnd->GetClientRect(&rect);

		CDC *pDC = pWnd->GetDC();

		image.Draw(pDC->m_hDC,rect);
		ReleaseDC(pDC);
		key = 1;
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CX_ImageSearchingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void ListFill(CListCtrl& c)
{
	int i;
	CString cs;

	CFile cf;
    cf.Open(_T("imagelist.txt"), CFile::modeRead);
    CArchive ar(&cf, CArchive::load);

	for(i = 0; i < 5613; i++)
	{
		ar.ReadString(cs);
		LPCTSTR p = cs;
		c.InsertItem(i,p);

	    c.EnsureVisible(i, TRUE);
        c.SetItemState(i, LVIS_SELECTED | LVIS_FOCUSED, 
                          LVIS_SELECTED | LVIS_FOCUSED);
        c.SetFocus( );
	}
}

void outputTheResult()
{
	DeleteFileA("result.html");//若已经存在result.html文件，则删去，然后重新生成
	CFile cf;
    cf.Open(_T("result.txt"), CFile::modeRead);
    CArchive ar(&cf, CArchive::load);
	CString cs;

	char y = 34;
	ofstream outFile("result.html",ios::app);          //生成一个result.html的网页文件
	outFile<<"<html>"<<endl;     //开始进行html编程
	outFile<<"<head><style type="<<y<<"text/css"<<y<<
		">body {background-color: aliceblue}";
	outFile<<"div#header {margin-left:150;height:150px;float:top;}"<<endl
		<<"div#content {margin-left:150;margin-top:60;width:100px;float:left;}"<<endl<<"div#center {margin-left:250;margin-top:60;float:left;}"<<endl<<"</style></head>"<<endl;
	outFile<<"<body background="<<y<<"content.jpg"<<y<<">"<<endl;
	outFile<<"<div id="<<y<<"header"<<y<<"><p><img src="<<y<<"head.jpg"<<y<<" /></p></div>"<<endl
		<<"<div id="<<y<<"content"<<y<<"><p><img src="<<y<<"team.gif"<<y<<" /></p></div>"<<endl;
	outFile<<"<div id="<<y<<"center"<<y<<"><p><img src="<<y<<"tail.gif"<<y<<"/></p>"<<endl;
	while(ar.ReadString(cs))                            //将搜索结果以链接的方式呈现出来，想看哪个网页，点击链接即可
	{
		LPCTSTR p = cs;
		//outFile<<"<a href="<<y<<p<<y<<"style="<<y<<"text-decoration:none"<<y<<">"<<p<<"</a>"<<endl<<"<br />";
		outFile<<"<p><img src="<<y<<p<<y<<"width = 400 align = right"<<" /></p>";
	}
	outFile<<"<p><img src="<<y<<"tail.gif"<<y<<"/></p></div></body>"<<endl<<"</html>"<<endl;         //进行最后的html编程   

	                                                   // 用chrome浏览器打开result.html网页文件 
    ShellExecute(0,_T("open"),_T("chrome.exe"),_T("result.html"),0,0);
}


void CX_ImageSearchingDlg::OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码

	NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;
    int m = pNMListView->iItem;
    int n = pNMListView->iSubItem;

    if(m >=0)
    {
      CString txt = x_listControl.GetItemText(m,n);
	  LPCTSTR pictureName = txt;
	  char n[30];
	  int i;
	  for(i = 0; ;i++)
	  {
		  if(pictureName[i] == '.')
			  break;
		  else
			  n[i] = pictureName[i];
	  }
	  n[i] = '.';
	  n[i+1] = 'J';
	  n[i+2] = 'P';
	  n[i+3] = 'E';
	  n[i+4] = 'G';
	  n[i+5] = '\0';

	  selected = n;
	  CImage image;
	  image.Load(n);
	  CRect rect;
	  int cx = image.GetWidth();
	  int cy = image.GetHeight();
	  similar_image_search(n);
	  GetDlgItem(IDC_PICTURE)->GetWindowRect(&rect);
	  ScreenToClient(&rect);
	  GetDlgItem(IDC_PICTURE)->MoveWindow(rect.left,rect.top,cx,cy,TRUE);

	  CWnd *pWnd = GetDlgItem(IDC_PICTURE);
	  pWnd->GetClientRect(&rect);

	  CDC *pDC = pWnd->GetDC();

	  image.Draw(pDC->m_hDC,rect);
	  ReleaseDC(pDC);
    }

	*pResult = 0;
}


void CX_ImageSearchingDlg::OnBnClickedCm()
{
	// TODO: 在此添加控件通知处理程序代码
	//DeleteFileA("result.txt");//若已经存在result.txt文件，则删去，然后重新生成
	outputTheResult();
}
