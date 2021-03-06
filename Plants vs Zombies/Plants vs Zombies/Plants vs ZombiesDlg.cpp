
// Plants vs ZombiesDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "Plants vs Zombies.h"
#include "Plants vs ZombiesDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/************定义变量*****************/
HWND hwnd;	//窗口句柄
DWORD pid;	//进程pid
HANDLE hprocess;	//进程句柄
//阳光基址和偏移
DWORD ba_addr = 0x00755E0C;	//阳光基址
DWORD offset1 = 0x868;	//偏移1
DWORD offset2 = 0x5578;	//偏移2
CString num_of_sun; // 修改后的阳光数值
/*************************************/

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CPlantsvsZombiesDlg 对话框



CPlantsvsZombiesDlg::CPlantsvsZombiesDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PLANTSVSZOMBIES_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPlantsvsZombiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPlantsvsZombiesDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK2, &CPlantsvsZombiesDlg::OnBnClickedOk2)
	ON_BN_CLICKED(IDOK, &CPlantsvsZombiesDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT3, &CPlantsvsZombiesDlg::OnEnChangeEdit3)
END_MESSAGE_MAP()


// CPlantsvsZombiesDlg 消息处理程序

BOOL CPlantsvsZombiesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPlantsvsZombiesDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPlantsvsZombiesDlg::OnPaint()
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
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPlantsvsZombiesDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



// 修改阳光
void CPlantsvsZombiesDlg::OnBnClickedOk2()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR *pstopstring;
	long long_num_of_sun;

	// 检测进程打开状态
	if (hprocess == NULL)
	{
		AfxMessageBox(TEXT("请先打开游戏进程"));
		return;
	}
	
	// 检测输入框是否输入
	if (num_of_sun.IsEmpty())
	{
		AfxMessageBox(TEXT("请先输入修改后的阳光值"));
		return;
	}
	
	// 检测输入是否为正整数
	char temp;
	for (int i = 0; i < num_of_sun.GetLength(); i++)
	{
		temp = num_of_sun.GetAt(i);
		if (temp < '0' || temp > '9')
		{
			AfxMessageBox(TEXT("阳光值为正整数"));
			return;
		}
	}
	
	// CString转换成long类型
	long_num_of_sun = wcstol(num_of_sun, &pstopstring, 10);
	
	DWORD ptemp;
	// 读取阳光地址并修改
	// 读取基址数据
	ReadProcessMemory(hprocess, (LPVOID)ba_addr, &ptemp, 4, 0);
	// 一级指针加偏移1
	ReadProcessMemory(hprocess, (LPVOID)(ptemp+offset1), &ptemp, 4, 0);
	// 修改二级指针加偏移2即阳光地址
	bool res = WriteProcessMemory(hprocess, (LPVOID)(ptemp + offset2), &long_num_of_sun, 4, 0);
	if (res == false)
	{
		AfxMessageBox(TEXT("修改失败"));
	}
	else
	{
		AfxMessageBox(TEXT("修改成功"));
	}
}


//打开游戏进程
void CPlantsvsZombiesDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	// 打开窗口句柄
	hwnd = FindWindowA(NULL, "Plants vs. Zombies");
	if (hwnd != NULL) 
	{
		// 获取窗口进程PID
		GetWindowThreadProcessId(hwnd, &pid);
		// 打开进程句柄
		hprocess = OpenProcess(PROCESS_ALL_ACCESS, FALSE,  pid);
		if (hprocess == NULL)
		{
			AfxMessageBox(TEXT("游戏进程打开失败"));
		}
		else
		{
			AfxMessageBox(TEXT("游戏进程打开成功"));
		}
	}
	else
	{
		AfxMessageBox(TEXT("游戏没有运行，或没有使用管理员权限运行修改器"));
	}
}


// 阳光值输入框
void CPlantsvsZombiesDlg::OnEnChangeEdit3()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT3, num_of_sun);
}
