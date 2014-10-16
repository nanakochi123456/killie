#include "stdafx.h"
#include "killie.h"

// http://oshiete.goo.ne.jp/qa/3243408.html
// 定数
#define SW_TITLE (1) // タイトルバーの取得
#define SW_CLASS (2) // クラス名の取得

LPCSTR	_cszAppName      = "ブラウザー強制終了ソフト";

// 独自の構造体を宣言
typedef struct enum_param_t {
	LPTSTR	lpBuff;				// 先頭ポインタ位置
	LPTSTR	lpTail;				// 現在ポインタ位置
//	LPTSTR	lpStop;				// 最終ポインタ位置
	UINT	lpLen;				// 文字数
	UINT	swOption;			// 汎用オプション値
	HWND	hWnds[MAX_PROCESS];	// ウィンドウハンドルのスタック
	UINT	count;				// ウィンドウの数
	DWORD	hWnds2[MAX_PROCESS];// プロセスハンドルのスタック
	UINT	count2;				// プロセスの数
} enum_param_t;

// ウインドウのふるいにかける処理
BOOL MyIsTopWindow( HWND hWnd, LPTSTR lpTitle, LPTSTR lpClass ) {
	if ( IsWindowVisible(hWnd) ){        // 可視状態か
		if ( GetWindow(hWnd,GW_OWNER) == NULL ){    // トップレベルウィンドウか
			if ( lstrlen(lpTitle) > 0 ){      // キャプションを持っているか
				if ( lstrcmp(lpClass,TEXT("Progman")) != 0 ){ // シェルでないか 
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

// ウィンドウのふるいにかける処理２

BOOL IsTitleMatch(LPTSTR str, LPTSTR str1) {
#if 0
	char buf[65536];
	int iResult;
	wsprintf(buf, "debug\n%s\n%s",str,str1);
	iResult = MessageBox(NULL, buf,
			_cszAppName, MB_ICONSTOP | MB_OK);
#endif

#if	KILL_IE == 1
	if (strcasestr(str, TITLE_IE))
    		if (strcasestr(str1, CLASS_IE))
			return TRUE;
#endif
#if	KILL_FIREFOX == 1
	if (strcasestr(str, TITLE_FIREFOX))
		if (strcasestr(str1, CLASS_FIREFOX))
			return TRUE;
#endif
#if	KILL_CHROME == 1
	if (strcasestr(str, TITLE_CHROME))
		if (strcasestr(str1, CLASS_CHROME))
				return TRUE;
#endif
#if	KILL_OPERA == 1
	if (strcasestr(str, NAME_OPERA))
		if (strcasestr(str1, NAME_OPERA))
				return TRUE;
#endif
#if	KILL_SAFARI == 1
	if (strcasestr(str1, CLASS_SAFARI))
			return TRUE;
#endif
#if	KILL_LUNASCAPE == 1
	if (strcasestr(str, NAME_LUNASCAPE))
		if (strcasestr(str1, NAME_LUNASCAPE))
				return TRUE;
#endif
#if	KILL_SLEIPNIR == 1
	if (strcasestr(str, NAME_SLEIPNIR))
		if (strcasestr(str1, NAME_SLEIPNIR))
				return TRUE;
#endif
	return FALSE;
}

// プロセス名のふるいにかける処理２

UINT IsProcesseMatch(LPTSTR str) {
#if	KILL_IE == 1
	if (strcasestr(str, PROC_IE))
		return 2;
#endif
#if	KILL_FIREFOX == 1
	if (strcasestr(str, PROC_FIREFOX))
		return 2;
#endif
#if	KILL_CHROME == 1
	if (strcasestr(str, PROC_CHROME))
		return 2;
#endif
#if	KILL_OPERA == 1
	if (strcasestr(str, PROC_OPERA))
		return 2;
#endif
#if	KILL_SAFARI == 1
	if (strcasestr(str, PROC_SAFARI))
		return 1;
#endif
#if	KILL_LUNASCAPE == 1
	if (strcasestr(str, PROC_LUNASCAPE))
		return 2;
#endif
#if	KILL_SLEIPNIR == 1
	if (strcasestr(str, PROC_SLEIPNIR))
		return 2;
#endif
	return 0;
}

BOOL CALLBACK EnumWndProc( HWND hWnd, LPARAM lParam ) {
	enum_param_t *p = (enum_param_t *)lParam;  // ここで構造体を受け取る
	TCHAR szWindowName[ TEXT_SIZE ];
	TCHAR szClassName[ TEXT_SIZE ];
 
	//ウィンドウテキストの取得
	GetWindowText( hWnd, szWindowName, sizeof(szWindowName) );
	//ウィンドウクラス名の取得
	GetClassName( hWnd, szClassName, sizeof(szClassName) );
 
	// 取得した情報を文字列に取得
	if ( MyIsTopWindow(hWnd,szWindowName,szClassName) ){
		if ( IsTitleMatch(szWindowName, szClassName) || IsTitleMatch(szClassName, szClassName)) {
			LPTSTR lpBuff;
  
			// ここで swOption で処理分岐
			switch ( p->swOption ){
				case SW_TITLE: lpBuff = szWindowName; break; // タイトルバーの取得
				case SW_CLASS: lpBuff = szClassName; break; // クラス名の取得
				default: return TRUE;
			}
			// バッファの残り容量をチェック
//			if ( (p->lpTail + lstrlen(lpBuff) + 1) < p->lpStop ){
			if ((UINT)lstrlen(lpBuff) + 1 < p->lpLen ) {
				p->lpTail += wsprintf( p->lpTail, TEXT("%s\n"), lpBuff );
			}
			p->hWnds[p->count]=hWnd;
			p->count++;
		}
	}
	return TRUE;
}

VOID GetProcessList( LPARAM lParam ) {
	enum_param_t *p = (enum_param_t *)lParam;  // ここで構造体を受け取る
	DWORD	dwReaded;
	DWORD	lpdwPid[1024];
	UINT	uCt;
//	LRESULT lResult;
	char	szFileName[TEXT_SIZE] = {'\0'};
//	char	szBuff[TEXT_SIZE];
//	int		iResult;
	int		iCount=0;

	if(EnumProcesses(lpdwPid, sizeof(lpdwPid), &dwReaded) == FALSE){
		return;
	}

	for(uCt = 0; uCt <= dwReaded/sizeof(DWORD); uCt++){
		HANDLE	hProcess;
		HMODULE hModule;
		DWORD	dwNeeded;
		BOOL	fResult;
		UINT	flg;
		hProcess = OpenProcess(
				PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
				FALSE, *(lpdwPid+uCt));
		if(hProcess){
			fResult = EnumProcessModules(
					hProcess, &hModule, sizeof(hModule), &dwNeeded);
			if(fResult == TRUE){
				GetModuleBaseName(
						hProcess, hModule, szFileName, sizeof(szFileName));

				if(flg=IsProcesseMatch(szFileName)) {

#if 0
	char buf[65536];
	int iResult;
	wsprintf(buf, "debug\n%s\n",szFileName);
	iResult = MessageBox(NULL, buf,
			_cszAppName, MB_ICONSTOP | MB_OK);
#endif

					// バッファの残り容量をチェック
					if(flg == 1) {	
//						if ( (p->lpTail + lstrlen(lpBuff) + 1) < p->lpStop ){
						if ((UINT)lstrlen(p->lpBuff) + 1 < p->lpLen ) {
							p->lpTail += wsprintf( p->lpTail, TEXT("%s\n"), szFileName );
						}
					}
					p->hWnds2[p->count2]=(DWORD)*(lpdwPid+uCt);
					p->count2++;
				}
			}
		}
	}
}

BOOL killprocess( LPARAM lParam ) {
	enum_param_t *p = (enum_param_t *)lParam;  // ここで構造体を受け取る
	UINT	loop;
	if(p->count > 0) {
		for(loop=0; loop < p->count; loop++) {
			SendMessage(p->hWnds[loop], WM_CLOSE, 0, 0);
			SendMessage(p->hWnds[loop], WM_DESTROY, 0, 0);
		}
	}
	return TRUE;
}

BOOL killprocess2( LPARAM lParam ) {
	enum_param_t *p = (enum_param_t *)lParam;  // ここで構造体を受け取る
	UINT	loop;
	if(p->count2 > 0) {
		for(loop=0; loop < p->count2; loop++) {
			DWORD	dwProcessId;
			HANDLE	hProcess;
			BOOL	fResult = FALSE;

			dwProcessId=p->hWnds2[loop];

			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
			if(hProcess != NULL){
				/* 強制終了 */
				fResult = TerminateProcess(hProcess, 0);
				WaitForSingleObject(hProcess, 50000);		/* プロセスの終了を待つ */
				CloseHandle(hProcess);
			}
		}
	}
	return TRUE;
}

// メイン関数
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpszCmdLine, int nCmdShow) {
	CHAR *szText;
	CHAR *szBuff;
	// enum_param_t prm; // 構造体を宣言
	enum_param_t *prm;
	LRESULT iResult;


	prm=malloc(sizeof(enum_param_t));
	szText=(CHAR *)malloc(sizeof(CHAR) * TEXT_SIZE * MAX_PROCESS + 1);
	szBuff=(CHAR *)malloc(sizeof(CHAR) * TEXT_SIZE * MAX_PROCESS + 1);

	// 構造体の初期化
	prm->lpBuff	= szText;
	prm->lpTail	= szText; // ここが書き込み位置
//	prm->lpStop	= szText + sizeof( szText );
	prm->lpLen	= TEXT_SIZE * MAX_PROCESS;
	prm->swOption = SW_TITLE; // 機能拡張用のオプション(自由に値をセット)
	prm->count = 0;
	prm->count2 = 0;

	// ウインドウの列挙開始
	EnumWindows( EnumWndProc, (LPARAM)prm ); 

	// プロセスの列挙開始
	GetProcessList( (LPARAM)prm ); 

	if(prm->count + prm->count2 == 0) {
		iResult = MessageBox(NULL, "強制終了対象アプリケーションが起動していません。",
			_cszAppName, MB_ICONSTOP | MB_OK);
	} else {
		wsprintf(szBuff,
				"以下のアプリケーションを強制終了します。\n%s", szText);
		iResult = MessageBox(NULL, szBuff,
							_cszAppName, MB_ICONSTOP | MB_YESNO | MB_DEFBUTTON2);
		if(iResult == IDYES) {
			killprocess((LPARAM)prm);
			killprocess2((LPARAM)prm);
		}
	}
	free(szBuff);
	free(szText);
	free(prm);
	return 0;
}
