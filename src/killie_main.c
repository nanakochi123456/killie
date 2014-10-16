/*
kill ie main
*/

#include "stdafx.h"
#include "killie.h"
#include "shellapi.h"
/* include
http://www.softist.com/programming/tasktray/tasktray.htm
*/

BOOL WINAPI Shell_NotifyIcon(DWORD dwMessage, PNOTIFYICONDATA lpData);

UINT iTrayMsg = RegisterWindowMessage("XXXXNotify");


void AddTrayIcon(void)
{
    NOTIFYICONDATA icndata;
    memset(&icndata, 0, sizeof(icndata));
    icndata.cbSize = sizeof(icndata);
    //自分のウィンドウのハンドルを代入する
    icndata.hWnd = ((CTestTrayApp*)AfxGetApp())->m_pMainWnd->GetSafeHwnd();
    //チップメッセージも文字列をコピーする
    strcpy(icndata.szTip, "何でも", strlen("何でも"));
    //アイコンをセットする
    icndata.hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
    //システムトレーからの通知メッセージをセットする
    icndata.uCallbackMessage = iTrayMsg;
    icndata.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    Shell_NotifyIcon(NIM_ADD, &icndata);
}
３．アイコンを削除します。 例： 
void RemoveTrayIcon(void)
{
    NOTIFYICONDATA icndata;
    memset(&icndata, 0, sizeof(icndata));
    icndata.cbSize = sizeof(icndata);
    icndata.hWnd = AfxGetApp()->m_pMainWnd->GetSafeHwnd();
    Shell_NotifyIcon(NIM_DELETE, &icndata);
}

LRESULT CXTrayDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
    if(message == iTrayMsg)
    {
        if(lParam == WM_LBUTTONDBLCLK)
        {
            if (((CTestTrayApp*)AfxGetApp())->m_bShowing)
            {
                ((CTestTrayApp*)AfxGetApp())->m_bShowing = FALSE;
                ((CTestTrayApp*)AfxGetApp())->m_pMainWnd->ShowWindow(SW_HIDE);
            }
            else
            {
                ((CTestTrayApp*)AfxGetApp())->m_bShowing = TRUE;
                ((CTestTrayApp*)AfxGetApp())->m_pMainWnd->ShowWindow(SW_SHOW);
            }
        }
    }
    return CDialog::DefWindowProc(message, wParam, lParam);
}


