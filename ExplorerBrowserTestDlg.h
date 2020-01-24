
// ExplorerBrowserTestDlg.h : header file
//

#pragma once


// CExplorerBrowserTestDlg dialog
class CExplorerBrowserTestDlg : public CDialogEx
{
// Construction
public:
  CExplorerBrowserTestDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_EXPLORERBROWSERTEST_DIALOG };
#endif

  protected:
  virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
  HICON m_hIcon;

  CComPtr<IExplorerBrowser> m_pExplorerBrowser;
  CButton m_btnBrowse;

  // Generated message map functions
  virtual BOOL OnInitDialog();

  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnDestroy();

  afx_msg void OnBrowse();

  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
  afx_msg void OnPaint();
  afx_msg HCURSOR OnQueryDragIcon();
  DECLARE_MESSAGE_MAP()

private:
  BOOL NavigateTo(LPCTSTR lpszPath);
};
