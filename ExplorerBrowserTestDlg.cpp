
// ExplorerBrowserTestDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "ExplorerBrowserTest.h"
#include "ExplorerBrowserTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static const CRect sc_rcMargin{ 16, 16, 96, 16 };

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
  CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_ABOUTBOX };
#endif

  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CExplorerBrowserTestDlg dialog



CExplorerBrowserTestDlg::CExplorerBrowserTestDlg(CWnd* pParent /*=nullptr*/)
  : CDialogEx(IDD_EXPLORERBROWSERTEST_DIALOG, pParent)
{
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CExplorerBrowserTestDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);

  DDX_Control(pDX, IDC_BUTTON1, m_btnBrowse);
}

BEGIN_MESSAGE_MAP(CExplorerBrowserTestDlg, CDialogEx)
  ON_WM_SIZE()
  ON_WM_DESTROY()
  ON_WM_SYSCOMMAND()
  ON_WM_PAINT()
  ON_WM_QUERYDRAGICON()
  ON_COMMAND(IDC_BUTTON1, OnBrowse)
END_MESSAGE_MAP()


// CExplorerBrowserTestDlg message handlers

BOOL CExplorerBrowserTestDlg::OnInitDialog()
{
  CDialogEx::OnInitDialog();

  // Add "About..." menu item to system menu.

  // IDM_ABOUTBOX must be in the system command range.
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

  // Set the icon for this dialog.  The framework does this automatically
  //  when the application's main window is not a dialog
  SetIcon(m_hIcon, TRUE);			// Set big icon
  SetIcon(m_hIcon, FALSE);		// Set small icon

  // TODO: Add extra initialization here

  // dbrookes, Create explorer browser...

  HRESULT hr = CoCreateInstance(CLSID_ExplorerBrowser, NULL, CLSCTX_INPROC, IID_PPV_ARGS(&m_pExplorerBrowser));
  if (FAILED(hr) || m_pExplorerBrowser == nullptr)
    return FALSE;

  CRect rcClient;
  GetClientRect(&rcClient);

  m_pExplorerBrowser->SetOptions(EBO_ALWAYSNAVIGATE);

  FOLDERSETTINGS fs;
  fs.fFlags = 0;
  fs.ViewMode = FVM_DETAILS;
  
  rcClient.DeflateRect(sc_rcMargin);
  hr = m_pExplorerBrowser->Initialize(m_hWnd, rcClient, &fs);

  if (FAILED(hr))
    return FALSE;

  // Navigate to C:/
  VERIFY(NavigateTo(_T("C:\\")));

  return TRUE;  // return TRUE  unless you set the focus to a control
}

void CExplorerBrowserTestDlg::OnSize(UINT nType, int cx, int cy)
  {
  CDialogEx::OnSize(nType, cx, cy);

  // dbrookes, resize explorer browser to client size with fixed margin.
  if (m_pExplorerBrowser)
    {
    CRect rc(sc_rcMargin.left, sc_rcMargin.top, 
             cx - sc_rcMargin.right, cy - sc_rcMargin.bottom);
    m_pExplorerBrowser->SetRect(NULL, rc);
    }
  }

void CExplorerBrowserTestDlg::OnDestroy()
  {
  CDialogEx::OnDestroy();

  // dbrookes, destroy explorer browser.
  if (m_pExplorerBrowser)
    m_pExplorerBrowser->Destroy();
  }

void CExplorerBrowserTestDlg::OnBrowse()
  {
  // dbrookes, open file explorer.
  // FIXME: Open in current directory.

  IFileDialog *pFileDialog = NULL;
  HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));
  if (SUCCEEDED(hr))
    {
    ASSERT(pFileDialog);

    DWORD dwOptions;
    hr = pFileDialog->GetOptions(&dwOptions);
    if (SUCCEEDED(hr))
      {
      pFileDialog->SetOptions(dwOptions | FOS_PICKFOLDERS);

      hr = pFileDialog->Show(m_hWnd);
      if (SUCCEEDED(hr))
        {
        // Retrieve picked folder name.
        IShellItem *pShellItem;
        LPTSTR lpszPath;
        hr = pFileDialog->GetResult(&pShellItem);
        if (SUCCEEDED(hr))
          {
          ASSERT(pShellItem);

          hr = pShellItem->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &lpszPath);
          if (SUCCEEDED(hr))
            {
            NavigateTo(lpszPath);
            }

          pShellItem->Release();
          }
        }
      }

    pFileDialog->Release();
    }
  }

void CExplorerBrowserTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CExplorerBrowserTestDlg::OnPaint()
{
  if (IsIconic())
  {
    CPaintDC dc(this); // device context for painting

    SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

    // Center icon in client rectangle
    int cxIcon = GetSystemMetrics(SM_CXICON);
    int cyIcon = GetSystemMetrics(SM_CYICON);
    CRect rect;
    GetClientRect(&rect);
    int x = (rect.Width() - cxIcon + 1) / 2;
    int y = (rect.Height() - cyIcon + 1) / 2;

    // Draw the icon
    dc.DrawIcon(x, y, m_hIcon);
  }
  else
  {
    CDialogEx::OnPaint();
  }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CExplorerBrowserTestDlg::OnQueryDragIcon()
{
  return static_cast<HCURSOR>(m_hIcon);
}

BOOL CExplorerBrowserTestDlg::NavigateTo(LPCTSTR lpszPath)
  {
  LPITEMIDLIST pidlBrowse = NULL;

  // dbrookes, Set explorer browser current folder.
  HRESULT hr = ::SHParseDisplayName(lpszPath, NULL, &pidlBrowse, 0, NULL);
  if (SUCCEEDED(hr))
    hr = m_pExplorerBrowser->BrowseToIDList(pidlBrowse, SBSP_ABSOLUTE);

  ::ILFree(pidlBrowse);

  return SUCCEEDED(hr);
  }

